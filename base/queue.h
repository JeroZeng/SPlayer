#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_LENGTH 8
#if 1
typedef pthread_mutex_t SLock;
typedef pthread_cond_t  SCond;
#endif

class CLock{
public:
    CLock(SLock &lock): lc(lock){
        pthread_mutex_lock(&lc);
    }
    ~CLock(){
        pthread_mutex_unlock(&lc);
    }
private:
    SLock &lc;
};

class SData{
public:
    SData *prev;
    SData *next;
    int num;
};

class SQueue{
public:
    SQueue();
    ~SQueue();

    void Push(SData* data);
    SData* Pop();
    void Flush(); 

private:
    SLock m_sLock;
    SCond m_sCond;
    SData *m_sHead;
    SData *m_sTail;
    int m_iSize;
};

#endif//_QUEUE_H_
