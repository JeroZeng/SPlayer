#include "H264Demuxer.h"

int H264Demuxer::Open(const char *url) {
    m_pFile = fopen(url, "r");
    if (m_pFile == (FILE*)NULL) {
        printf("Cant open input 264 file\n");
        return 1;
    }
    memBuf = (uint8_t*)malloc(BUFFER_2M * sizeof(uint8_t));
    return 0;
}

int GetOneFrame(SBucket *bucket) {
    
}
