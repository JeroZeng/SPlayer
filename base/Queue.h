#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#define _DEBUG_
#define MAX_QUEUE_LENGTH 8
#define MALLOC malloc
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
public:
    SNode();
    ~SNode();
};

class SQueue{
public:
    SQueue(int len);
    ~SQueue();

    void Push(SBucket **bucket);
    void Pop(SBucket **bucket);
    void Flush(); 

private:
    SLock m_sLock;
    SCond m_sCond;
    SNode *m_sWriter;
    SNode *m_sReader;
};

#endif//_QUEUE_H_
