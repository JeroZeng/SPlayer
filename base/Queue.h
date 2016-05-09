#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#include <strings.h>
#endif

#define _DEBUG_
#define MAX_QUEUE_LENGTH 8
#define MALLOC malloc
#ifdef _WIN32
typedef CRITICAL_SECTION   SLock;
typedef CONDITION_VARIABLE SCond;
#else
typedef pthread_mutex_t SLock;
typedef pthread_cond_t  SCond;
#endif

class CLock{
public:
    CLock(SLock &lock): lc(lock){
#ifdef _WIN32
        EnterCriticalSection(&lc);
#else
        pthread_mutex_lock(&lc);
#endif
    }
    ~CLock(){
#ifdef _WIN32
        LeaveCriticalSection(&lc);
#else
        pthread_mutex_unlock(&lc);
#endif
    }
private:
    SLock &lc;
};

class SBucket {
public:
    unsigned char *data;
    int   size;
public:
    SBucket(){ data = NULL; size = 0; }
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

    int Push(SBucket **bucket);
    void Pop(SBucket **bucket);
    void StopReader();
    void Flush(); 

private:
    SLock m_sLock;
    SCond m_sCond;
    SNode *m_sWriter;
    SNode *m_sReader;
};

#endif//_QUEUE_H_
