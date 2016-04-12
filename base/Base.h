#ifndef _BASE_H_
#define _BASE_H_
#include <pthread.h>
#define USING_BASE using Base::Resume; \
                   using Base::Pause; \
                   using Base::Stop;

typedef enum {
    StatusPlay  = 0,
    StatusPause = 1,
    StatusStop  = 2
} EStatus;

class Base {
public:
    Base();
    ~Base();
    virtual int Start();
    virtual int Pause();
    virtual int Stop();
    virtual int Wait();
    virtual int Resume();
    void SetStatus(EStatus vs);
    EStatus GetStatus();
public:
    pthread_t m_thread;
    unsigned int  m_wait;
    EStatus m_eStatus;
};
#endif//_BASE_H_
