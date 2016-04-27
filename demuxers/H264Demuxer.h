#ifndef _H264_DEMUXER_H_
#define _H264_DEMUXER_H_

#include "../base/demuxer.h"

#define BUFFER_2M (2 << 20)

class H264Demuer: public Demuxer {
public:

public:
    int Open(const char *url);
    int GetOneFrame(SBucket *bucket);
    void Reset();

private:
    FILE *m_pFile;
    unsigned char *mem_buf;
};

#endif//_H264_DEMUXER_H_
