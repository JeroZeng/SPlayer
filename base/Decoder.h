#ifndef _DECODER_H_
#define _DECODER_H_

#include "Queue.h"
#include "Render.h"

class Decoder{

public:
    SQueue *m_sQueue;
    RenderQueue *m_sRenderQueue;

public:
    Decoder();
    ~Decoder();
    void Init();
    int Start(SQueue *queue);
    int WaitStreamEnd();

    virtual int Recieve(char *dt);
    virtual int DecodeOneFrame(SBucket *bucket);

private:
    static void* Loop(void *arg);

private:
   pthread_t m_pThreadDecoder;

};

#endif//_DECODER_H_
