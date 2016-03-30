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

class SBucket {
public:
    char *data;
    int   size;
};

class SNode{
public:
    SBucket *bucket;
    SNode *next;
};

class SQueue{
public:
    SQueue(int len);
    ~SQueue();

    void Push(SBucket* bucket);
    SBucket* Pop();
    void Flush(); 

private:
    SLock m_sLock;
    SCond m_sCond;
    SNode *m_sWriter;
    SNode *m_sReader;
};

#endif//_QUEUE_H_
