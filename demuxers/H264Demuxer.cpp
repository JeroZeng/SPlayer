#include "H264Demuxer.h"

H264Demuxer::H264Demuxer() {
    m_bReadFile = true;
    m_iUsedSize = 0;
    m_iNextFrameSize = 0;
    m_dFPS = 25.0;
    m_iWidth = 176;
    m_iHeight = 144;
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
    for( i=0; i<BUFFER_SIZE-m_iUsedSize; i++) {
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
    if (i == BUFFER_SIZE-m_iUsedSize-1) {
        m_bReadFile = true;
        fseek(m_pFile, m_iUsedSize-BUFFER_SIZE, SEEK_CUR);
        m_iUsedSize = 0;
        return 0;
    }
    m_iNextFrameSize = i;
    return m_iNextFrameSize;
}

int H264Demuxer::GetOneFrame(SBucket *bucket) {
    while (true) {
        if (m_bReadFile) {
            fread(m_MemBuf, BUFFER_SIZE, 1, m_pFile);
            m_bReadFile = false;
        }
        if (m_iNextFrameSize == 0) {
            GetNextFrameSize();
            if (m_bReadFile)
                continue;
        }
        int curFrameSize = m_iNextFrameSize;
        bucket->size = curFrameSize;
        memcpy(bucket->data, m_MemBuf+m_iUsedSize, bucket->size);
        m_iUsedSize += curFrameSize;
        GetNextFrameSize();
        if (m_bReadFile)
            continue;
        return m_iNextFrameSize;
    }
}

void H264Demuxer::Reset() {

}
