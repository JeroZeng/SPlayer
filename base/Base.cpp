#include <stdio.h>
#include "Base.h"

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
