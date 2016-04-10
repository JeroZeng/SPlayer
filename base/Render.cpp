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

int Render::WaitStreamEnd() {
    pthread_join(m_thread, NULL);
    return 0;
}

void* Render::Loop(void *arg) {

    Render *render = (Render*)arg;
    SBucket *bucket = new SBucket();
    render->m_sRenderQueue->Pop(&bucket);
    while(!render->ShouldExit() && bucket->size) {
        render->m_sRenderQueue->Pop(&bucket);
        render->Draw(bucket);
    }
    render->m_sRenderQueue->StopReader();
    if (bucket->size == 0) {
        delete bucket;
    }
    return NULL;
}

int Render::Draw(SBucket *bucket) {
    return 0;
}

int Render::Stop() {
    return pthread_kill(m_thread, SIGQUIT);
}

bool Render::ShouldExit() {
    return false;
}

int Render::Exit() {
    return 0;
}
