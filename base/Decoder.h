#ifndef _DECODER_H_
#define _DECODER_H_

#include "Queue.h"
#include "Render.h"

#define RQ_SIZE 6

class Decoder{

public:
    SQueue *m_sQueue;
    RenderQueue *m_sRenderQueue;
    int m_iWidth;
    int m_iHeight;

public:
    Decoder();
    ~Decoder();
    void Init();
    int Start(SQueue *queue);
    int Stop();
    int WaitStreamEnd();
    void ClearMem();

    virtual int Recieve(char *dt);
    virtual int DecodeOneFrame(SBucket *db, SBucket *rb);

private:
    static void* Loop(void *arg);

private:
    pthread_t m_pThreadDecoder;
    char *m_MemBar[RQ_SIZE+2];

};

#endif//_DECODER_H_
