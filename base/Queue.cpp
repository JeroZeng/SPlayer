#include "Queue.h"

SQueue::SQueue(int len) {
    int i = 0;
    pthread_mutex_init(&m_sLock, NULL);
    pthread_cond_init(&m_sCond, NULL);
    pthread_mutex_lock(&m_sLock);
    SNode *xNode = new SNode();
    m_sWriter = m_sReader = xNode;
    for(i=1; i<len; i++) {
        xNode->next = new SNode();
        xNode = xNode->next;
    }
    xNode->next = m_sWriter;
    pthread_mutex_unlock(&m_sLock);
}

SQueue::~SQueue() {
    //CLock lock(&m_sLock);
    //TODO Release SBuckets if exist.
    pthread_mutex_destroy(&m_sLock);
    pthread_cond_destroy(&m_sCond);
}

void SQueue::Push(SBucket *bucket) {
    CLock lock(m_sLock);
    while(m_sWriter->next == m_sReader) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    m_sWriter->bucket = bucket;
    m_sWriter = m_sWriter->next;
    pthread_cond_signal(&m_sCond);
}

SBucket* SQueue::Pop() {
    CLock lock(m_sLock);
    while(m_sReader == m_sWriter) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    SBucket *bucket = m_sReader->bucket;
    m_sReader = m_sReader->next;
    pthread_cond_signal(&m_sCond);
    return bucket;
}

void SQueue::Flush() {
    CLock lock(m_sLock);

}
