#ifndef _DECODER_H_
#define _DECODER_H_

#include "Queue.h"

class Decoder{

public:
    SQueue *m_sQueue;

public:
    Decoder();
    ~Decoder();
    void Init();
    int Start(SQueue *queue);
    int WaitThread();

    virtual int Recieve(char *dt);

private:
    static void* Loop(void *arg);

private:
   pthread_t m_pThreadDecoder;

};

#endif//_DECODER_H_
