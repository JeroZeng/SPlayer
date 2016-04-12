#include <signal.h>
#include <stdio.h>
#include "Base.h"

Base::Base() {
    m_wait = 0;
}

Base::~Base() {
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
    m_wait = 1;
    //while(__sync_bool_compare_and_swap(&m_wait, 1, 1));
    //m_wait = 0;
    while (m_wait == 1);
    if (m_eStatus == StatusStop)
        return 0;
    return 1;
}

int Base::Resume() {
    m_wait = 0;
    m_eStatus = StatusPlay; 
    return 0;
}
