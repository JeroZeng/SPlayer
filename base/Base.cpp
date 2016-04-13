#include <signal.h>
#include <stdio.h>
#include "Base.h"

Base::Base() {
    sem_unlink("SPlayerPause");
    m_wait = sem_open("SPlayerPause", O_CREAT, 0644, 0);
}

Base::~Base() {
    sem_close(m_wait);
    sem_unlink("SPlayerPause");
    printf("------------------->remove signal\n");
}

int Base::Start() {
    return 0;
}

int Base::Pause() {
    if(m_eStatus == StatusPause) {
        return Resume();
    }
    m_eStatus = StatusPause;
    return 0;
}

int Base::Stop() {
    Resume();
    m_eStatus = StatusStop;
    return 0;
}

void Base::SetStatus(EStatus st) {
    m_eStatus = st;
}

EStatus Base::GetStatus() {
    return m_eStatus;
}

int Base::Wait() {
    //while(__sync_bool_compare_and_swap(&m_wait, 1, 1));
    //m_wait = 0;
    sem_wait(m_wait);
    if (m_eStatus == StatusStop)
        return 0;
    return 1;
}

int Base::Resume() {
    if (m_eStatus == StatusPause)
        sem_post(m_wait);
    m_eStatus = StatusPlay; 
    return 0;
}

int Base::WaitThreadClose() {
    pthread_join(m_thread, NULL);
    return 0;
}
