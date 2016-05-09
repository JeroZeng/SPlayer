#include "Queue.h"

SNode::SNode() {
    bucket = new SBucket();
}

SNode::~SNode() {
    delete bucket;
}

SQueue::SQueue(int len) {
    int i = 0;
    
#ifdef _WIN32
    InitializeConditionVariable(&m_sCond);
    InitializeCriticalSection(&m_sLock);
    EnterCriticalSection(&m_sLock);
#else
    pthread_cond_init(&m_sCond, NULL);
    pthread_mutex_init(&m_sLock, NULL);
    pthread_mutex_lock(&m_sLock);
#endif
    SNode *xNode = new SNode();
    m_sWriter = m_sReader = xNode;
    for(i=1; i<len; i++) {
        xNode->next = new SNode();
        xNode = xNode->next;
    }
    xNode->next = m_sWriter;
#ifdef _WIN32
    LeaveCriticalSection(&m_sLock);
#else
    pthread_mutex_unlock(&m_sLock);
#endif
}

SQueue::~SQueue() {
    //CLock lock(&m_sLock);
    //TODO Release SBuckets if exist.
    SNode *next = m_sWriter->next;
    SNode *head = m_sWriter;
    while (next != head) {
        delete m_sWriter;
        m_sWriter = next;
        next = m_sWriter->next;
    }
    delete m_sWriter;
#ifdef _WIN32
    DeleteCriticalSection(&m_sLock);
#else
    pthread_mutex_destroy(&m_sLock);
    pthread_cond_destroy(&m_sCond);
#endif
}

int SQueue::Push(SBucket **bucket) {
    CLock lock(m_sLock);
    if (m_sReader == NULL) {
        return -1;
    }
    while(m_sWriter->next == m_sReader) {
#ifdef _WIN32
        SleepConditionVariableCS(&m_sCond, &m_sLock, INFINITE);
#else
        pthread_cond_wait(&m_sCond, &m_sLock);
#endif
    }
    SBucket *tBucket = m_sWriter->bucket;
    m_sWriter->bucket = *bucket;
    *bucket = tBucket;
    m_sWriter = m_sWriter->next;
#ifdef _WIN32
    WakeConditionVariable(&m_sCond);
#else
    pthread_cond_signal(&m_sCond);
#endif
    return 0;
}

void SQueue::Pop(SBucket **bucket) {
    CLock lock(m_sLock);
    while(m_sReader == m_sWriter) {
#ifdef _WIN32
        SleepConditionVariableCS(&m_sCond, &m_sLock, INFINITE);
#else
        pthread_cond_wait(&m_sCond, &m_sLock);
#endif
    }
    SBucket *tBucket = m_sReader->bucket;
    m_sReader->bucket = *bucket;
    *bucket = tBucket;
    m_sReader = m_sReader->next;
#ifdef _WIN32
    WakeConditionVariable(&m_sCond);
#else
    pthread_cond_signal(&m_sCond);
#endif
}

void SQueue::StopReader() {
    CLock lock(m_sLock);
    m_sReader = NULL;
#ifdef _WIN32
    WakeConditionVariable(&m_sCond);
#else
    pthread_cond_signal(&m_sCond);
#endif
}

void SQueue::Flush() {
    CLock lock(m_sLock);

}
