#ifndef _H264_DEMUXER_H_
#define _H264_DEMUXER_H_

#include "../base/demuxer.h"

#ifdef _WIN32
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
#endif

#define BUFFER_SIZE (1 << 26)

class H264Demuxer: public Demuxer {
public:

public:
    H264Demuxer();
    int Open(const char *url);
    int GetOneFrame(SBucket *bucket);
    inline int GetNextFrameSize();
    void Reset();

private:
    FILE *m_pFile;
    unsigned char *m_MemBuf;
    int m_iReadSize;
    bool m_bReadFile;
    uint32_t m_iUsedSize;
    uint32_t m_iNextFrameSize;
};

#endif//_H264_DEMUXER_H_
