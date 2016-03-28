#include "queue.h"

SQueue::SQueue() {

    pthread_mutex_init(&m_sLock, NULL);
    pthread_cond_init(&m_sCond, NULL);
    pthread_mutex_lock(&m_sLock);
    m_sHead = NULL;
    m_sTail = NULL;
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
    while(m_iSize == MAX_QUEUE_LENGTH) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    if (m_iSize == 0) {
        data->prev = NULL;
        data->next = NULL;
        m_sHead = data;
        m_sTail = data;
    } else {
        m_sHead->prev = data;
        data->next = m_sHead;
        data->prev = NULL;
        m_sHead = data;
    }
    m_iSize++;
    pthread_cond_signal(&m_sCond);
}

SData* SQueue::Pop() {
    CLock lock(m_sLock);
    while(m_iSize == 0) {
        pthread_cond_wait(&m_sCond, &m_sLock);
    }
    SData *data = NULL;
    if (m_iSize > 0) {
        data = m_sTail;
        if (m_sTail->prev != NULL) {
            m_sTail = m_sTail->prev;
            m_sTail->next = NULL;
        } else {
            m_sTail = NULL;
        }
        m_iSize--;
        pthread_cond_signal(&m_sCond);
    }
    return data;
}

void SQueue::Flush() {
    CLock lock(m_sLock);

}
