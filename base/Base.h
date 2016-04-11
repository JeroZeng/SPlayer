#ifndef _BASE_H_
#define _BASE_H_
#include <pthread.h>
#define USING_BASE using Base::Start; \
                   using Base::Pause; \
                   using Base::Stop; \
                   using Base::SetStatus; \
                   using Base::GetStatus

typedef enum {
    StatusPlay  = 0,
    StatusPause = 1,
    StatusStop  = 2
} EStatus;

class Base {
public:
    virtual int Start();
    virtual int Pause();
    virtual int Stop();
    void SetStatus(EStatus vs);
    EStatus GetStatus();
public:
    pthread_t m_thread;
    EStatus m_eStatus;
};
#endif//_BASE_H_
