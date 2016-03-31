#ifndef _RAW_YUV_H_
#define _RAW_YUV_H_

#include "../base/Demuxer.h"

class RawYUV: public Demuxer {
public:
    int m_iFrameSize;
    bool m_bLoop;

public:
    RawYUV();
    ~RawYUV();
    int Open(const char *url);
    int GetOneFrame(SBucket *bucket);
    int SetVideoProperty();


private:
    FILE *m_pFile;

};

#endif//_RAW_YUV_H_
