#include "Queue.h"

SQueue::SQueue(int len) {
    int i = 0;
    pthread_mutex_init(&m_sLock, NULL);
    pthread_cond_init(&m_sCond, NULL);
    pthread_mutex_lock(&m_sLock);
    m_sXer = new SNode();
    m_sWriter = m_sReader = m_sXer;
    for(i=1; i<len; i++) {
        m_sXer->next = new SNode();
        m_sXer = m_sXer->next;
    }
    m_sXer->next = m_sWriter;
    pthread_mutex_unlock(&m_sLock);
}

SQueue::~SQueue() {
    //CLock lock(&m_sLock);
    //TODO Release SDatas if exist.
    pthread_mutex_destroy(&m_sLock);
    pthread_cond_destroy(&m_sCond);
}

void SQueue::Push(SData *data) {
    CLock lock(m_sLock);
    while(m_sWriter->next == m_sReader) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    m_sWriter->data = data;
    m_sWriter = m_sWriter->next;
    pthread_cond_signal(&m_sCond);
}

SData* SQueue::Pop() {
    CLock lock(m_sLock);
    while(m_sReader == m_sWriter) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    SData *data = m_sReader->data;
    m_sReader = m_sReader->next;
    pthread_cond_signal(&m_sCond);
    return data;
}

void SQueue::Flush() {
    CLock lock(m_sLock);

}
