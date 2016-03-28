#ifndef _MUTEX__H_
#define _MUTEX__H_

#ifdef _UNIX_
#include <pthread.h>
#endif

class Mutex{
public:
    Mutex();
    ~Mutex();

private:
#ifdef _UNIX_
    pthread_mutex_t m_Mutex;
#endif
};

#endif//_MUTEXE_H_
