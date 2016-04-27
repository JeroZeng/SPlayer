#include "H264Demuxer.h"

H264Demuxer::H264Demuxer() {
    m_bReadFile = true;
    m_iUsedSize = 0;
    m_iNextFrameSize = 0;
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

int H264Demuxer::GetOneFrame(SBucket *bucket) {
    int i = 0;
    if (m_bReadFile) {
        fread(m_MemBuf, BUFFER_SIZE, 1, m_pFile);
        m_bReadFile = false;
    }
    if (m_iNextFrameSize == 0) {
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
        m_iNextFrameSize = i;
    }
    bucket->size = m_iNextFrameSize;
    m_iUsedSize++;
    memcpy(bucket->data, m_MemBuf, m_iNextFrameSize);
    m_iUsedSize += m_iNextFrameSize;    
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
    m_iNextFrameSize = i;
    return i;
}

void H264Demuxer::Reset() {

}
