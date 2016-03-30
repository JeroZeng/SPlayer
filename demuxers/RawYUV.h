#ifndef _RAW_YUV_H_
#define _RAW_YUV_H_

#include "../base/Demuxer.h"

class RawYUV: public Demuxer {
public:
    int m_iWidth;
    int m_iHeight;
    int m_iFrameSize;

public:
    RawYUV();
    ~RawYUV();
    int Open(const char *url);
    int GetOneFrame(char *data);
    int SetVideoProperty();


private:
    FILE *m_pFile;

};

#endif//_RAW_YUV_H_
