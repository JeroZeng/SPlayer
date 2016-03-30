#ifndef _DEMUXER_H_
#define _DEMUXER_H_

#include "Queue.h"

class Demuxer
{
public:
    SQueue *m_sQueue;
public:
    ~Demuxer();
    virtual int Open(const char *url);
    virtual int GetOneFrame(SBucket *bucket);
    int Start();
    int WaitStreamEnd();
    void SetFrameSize(int size);
    int GetFrameSize();
private:
    static void* Loop(void *arg);
private:
    int m_iFrameSize;
    pthread_t m_pThreadDemuxer;

};

#endif//_DEMUXER_H_
