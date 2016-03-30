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
    virtual int GetOneFrame(char *data);
    int Start();
    int WaitStreamEnd();
private:
    static void* Loop(void *arg);
private:
    pthread_t m_pThreadDemuxer;

};

#endif//_DEMUXER_H_
