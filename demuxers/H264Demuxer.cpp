#include "H264Demuxer.h"

H264Demuxer::H264Demuxer() {
    m_bReadFile = true;
    m_iUsedSize = 0;
    m_iNextFrameSize = 0;
    m_dFPS = 29.90f;
    m_iWidth = 0;
    m_iHeight = 0;
    m_iReadSize = BUFFER_SIZE;
}

int H264Demuxer::Open(const char *url) {
    m_pFile = fopen(url, "r");
    if (m_pFile == (FILE*)NULL) {
        printf("Cant open input 264 file\n");
        return 1;
    }
    m_MemBuf = (uint8_t*)malloc(BUFFER_SIZE * sizeof(uint8_t));
    return 0;
}

int H264Demuxer::GetNextFrameSize() {
    int i = 0;
    m_iUsedSize++;
    for( i=0; i<(int)(m_iReadSize-m_iUsedSize); i++) {
        if ((m_MemBuf[m_iUsedSize+i] == 0 &&
             m_MemBuf[m_iUsedSize+i+1] == 0 &&
             m_MemBuf[m_iUsedSize+i+2] == 0 &&
             m_MemBuf[m_iUsedSize+i+3] == 1) ||
            (m_MemBuf[m_iUsedSize+i] == 0 &&
             m_MemBuf[m_iUsedSize+i+1] == 0 &&
             m_MemBuf[m_iUsedSize+i+2] == 1)) {
            if (i<4)
                continue;
            break;
        }
    }
    if (i == m_iReadSize-m_iUsedSize) {
        if (m_iReadSize != BUFFER_SIZE) {
            m_iNextFrameSize = i;
            return 0;
        }
        m_bReadFile = true;
        m_iUsedSize--;
        fseek(m_pFile, m_iUsedSize-BUFFER_SIZE, SEEK_CUR);
        m_iUsedSize = 0;
        m_iNextFrameSize = 0;
        return 0;
    }
    m_iNextFrameSize = i;
    return m_iNextFrameSize;
}

int H264Demuxer::GetOneFrame(SBucket *bucket) {
    //sleep(3);
    bool bFlag= false;
    while (true) {
        if (m_bReadFile) {
            int pos = ftell(m_pFile);
            int size = fread(m_MemBuf, BUFFER_SIZE, 1, m_pFile);
            if (size) {
                m_bReadFile = false;
                m_iReadSize = BUFFER_SIZE;
            } else {
                size = ftell(m_pFile);
                size -= pos;
                m_iReadSize = size;
                fseek(m_pFile, -1*size, SEEK_END);
                size = fread(m_MemBuf, size, 1, m_pFile);
                m_bReadFile = false;
            }
        }
        if (m_iNextFrameSize == 0) {
            GetNextFrameSize();
            if (m_bReadFile)
                continue;
        }
        if (bFlag)
           return m_iNextFrameSize;
        int curFrameSize = m_iNextFrameSize;
        bucket->size = curFrameSize;
        memcpy(bucket->data, m_MemBuf+m_iUsedSize, bucket->size);
        //printf("%d\t%d\t%d\t%d\t", bucket->data[3], bucket->data[4], bucket->data[5],     bucket->data[6]);
        m_iUsedSize += curFrameSize;
        GetNextFrameSize();
        if (m_bReadFile) {
            bFlag = true;
            continue;
        }
        return m_iNextFrameSize;
    }
}

void H264Demuxer::Reset() {
    if (m_pFile != NULL) {
        fseek(m_pFile, 0, SEEK_SET);
    }
    m_bReadFile = true;
    m_iUsedSize = 0;
    m_iNextFrameSize = 0;
    m_iReadSize = BUFFER_SIZE;
}
