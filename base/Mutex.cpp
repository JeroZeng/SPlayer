#include "Mutex.h"

Mutex::Mutex(){
    pthread_mutex_init(&m_Mutex, NULL);
    pthread_mutex_lock(&m_Mutex);
}

Mutex::~Mutex(){
    pthread_mutex_destroy(&m_Mutex);
}
