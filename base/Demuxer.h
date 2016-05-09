#ifndef _DEMUXER_H_
#define _DEMUXER_H_

#include "Base.h"
#include "Queue.h"

class Demuxer : private Base
{
public:
    SQueue *m_sQueue;
    int m_iWidth;
    int m_iHeight;
    double m_dFPS;
public:
    Demuxer();
    ~Demuxer();

    USING_BASE;

    virtual int Open(const char *url);
    virtual int GetOneFrame(SBucket *bucket);
    int Start();
    int Stop();
    void SetFrameSize(int size);
    int GetFrameSize();
    void ClearMem();
    virtual void Reset();
private:
    static void* Loop(void *arg);
private:
    int m_iFrameSize;
    unsigned char *m_MemBar[2][5];
};

#endif//_DEMUXER_H_
