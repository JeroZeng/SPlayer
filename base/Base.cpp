#include <signal.h>
#include <stdio.h>
#include "Base.h"

Base::Base() {
    sigemptyset(&m_wait);
    sigaddset(&m_wait, SIGUSR1);
}

Base::~Base() {
}

int Base::Start() {
    return 0;
}

int Base::Pause() {
    m_eStatus = StatusPause;
    return 0;
}

int Base::Stop() {
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
    sigsuspend(&m_wait);
    return 0;
}

int Base::Resume() {
    printf("--------->Resume-----------\n");
    raise(SIGUSR1);
    return 0;
}
