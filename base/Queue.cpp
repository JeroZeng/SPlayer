#include "Queue.h"

SNode::SNode() {
    bucket = new SBucket();
}

SNode::~SNode() {
    delete bucket;
}

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
    SNode *next = m_sWriter->next;
    SNode *head = m_sWriter;
    while (next != head) {
        delete m_sWriter;
        m_sWriter = next;
        next = m_sWriter->next;
    }
    delete m_sWriter;
    pthread_mutex_destroy(&m_sLock);
    pthread_cond_destroy(&m_sCond);
}

int SQueue::Push(SBucket **bucket) {
    CLock lock(m_sLock);
    if (m_sReader == NULL) {
        return -1;
    }
    while(m_sWriter->next == m_sReader) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    SBucket *tBucket = m_sWriter->bucket;
    m_sWriter->bucket = *bucket;
    *bucket = tBucket;
    m_sWriter = m_sWriter->next;
    pthread_cond_signal(&m_sCond);
    return 0;
}

void SQueue::Pop(SBucket **bucket) {
    CLock lock(m_sLock);
    while(m_sReader == m_sWriter) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    SBucket *tBucket = m_sReader->bucket;
    m_sReader->bucket = *bucket;
    *bucket = tBucket;
    m_sReader = m_sReader->next;
    pthread_cond_signal(&m_sCond);
}

void SQueue::StopReader() {
    CLock lock(m_sLock);
    m_sReader = NULL;
    pthread_cond_signal(&m_sCond);

}

void SQueue::Flush() {
    CLock lock(m_sLock);

}
