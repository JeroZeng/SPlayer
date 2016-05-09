#include <signal.h>
#include <stdio.h>
#include "Base.h"

Base::Base() {
#ifdef _WIN32
    m_wait = CreateSemaphore(NULL, 0, 1, "SPlayerPause");
#else
    sem_unlink("SPlayerPause");
    m_wait = sem_open("SPlayerPause", O_CREAT, 0644, 0);
#endif
}

Base::~Base() {
#ifdef _WIN32
    CloseHandle(m_wait);
#else
    sem_close(m_wait);
    sem_unlink("SPlayerPause");
#endif
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
#ifdef _WIN32
    WaitForSingleObject(m_wait, INFINITE);
#else
    sem_wait(m_wait);
#endif
    if (m_eStatus == StatusStop)
        return 0;
    return 1;
}

int Base::Resume() {
    if (m_eStatus == StatusPause)
#ifdef _WIN32
        ReleaseSemaphore(m_wait, 1, NULL);
#else
        sem_post(m_wait);
#endif
    m_eStatus = StatusPlay; 
    return 0;
}

int Base::WaitThreadClose() {
#ifdef _WIN32
    WaitForSingleObject(m_wait, INFINITE);
#else
    pthread_join(m_thread, NULL);
#endif
    return 0;
}
