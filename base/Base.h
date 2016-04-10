#ifndef _BASE_H_
#define _BASE_H_
#include <pthread.h>
#define USING_BASE using Base::Start; \
                   using Base::Pause; \
                   using Base::Stop
class Base {
public:
    virtual int Start();
    virtual int Pause();
    virtual int Stop();
public:
    pthread_t m_thread;
};
#endif//_BASE_H_
