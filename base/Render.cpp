#include "Render.h"

RenderQueue::RenderQueue(int len):
    SQueue(len) {
}

Render::Render() {

}

Render::~Render() {

    if (m_sRenderQueue != NULL) {
        delete m_sRenderQueue;
    }

}

int Render::Init() {
    return 0;
}

int Render::Start(RenderQueue *queue) {

    m_sRenderQueue = queue;
    pthread_create(&m_thread, NULL, Render::Loop, (void*)this);
    return 0;
}

void* Render::Loop(void *arg) {

    Render *render = (Render*)arg;
    render->SetStatus(StatusPlay);
    SBucket *bucket = new SBucket();
    render->m_sRenderQueue->Pop(&bucket);
    while(bucket->size) {
        EStatus st = render->GetStatus();
        if (st == StatusStop) {
            break;
        } else
        if (st == StatusPause) {
            if (render->Wait())
                render->SetStatus(StatusPlay);
        }
        render->m_sRenderQueue->Pop(&bucket);
        render->Draw(bucket);
    }
    render->m_sRenderQueue->StopReader();
    render->ClearScreen();
    delete bucket;
    return NULL;
}

int Render::Draw(SBucket *bucket) {
    return 0;
}

int Render::ClearScreen() {
    return 0;
}

int Render::Exit() {
    return 0;
}
