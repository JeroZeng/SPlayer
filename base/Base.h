#ifndef _BASE_H_
#define _BASE_H_
#if defined(_WIN32)
#include <windows.h>
typedef HANDLE Thread_t;
typedef HANDLE Sem_t;
#else
#include <pthread.h>
#include <semaphore.h>
typedef pthread_t SThread;
typedef sem_t Sem-t;
#endif
#define RES_OK 0
#define USING_BASE using Base::Resume; \
                   using Base::Pause; \
                   using Base::Stop; \
                   using Base::WaitThreadClose;

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
    virtual int WaitThreadClose();
public:
    Thread_t m_thread;
    Sem_t  *m_wait;
    EStatus m_eStatus;
};
#endif//_BASE_H_
