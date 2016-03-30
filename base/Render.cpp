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

void Render::Init() {

}

int Render::Start(RenderQueue *queue) {

    m_sRenderQueue = queue;
    pthread_create(&m_pThreadRender, NULL, Render::Loop, (void*)this);
    return 0;
}

int Render::WaitStreamEnd() {
    pthread_join(m_pThreadRender, NULL);
    return 0;
}

int Render::Draw(char *yuv, int width, int height) {

    return 0;
}

void* Render::Loop(void *arg) {

    Render *render = (Render*)arg;
    SBucket *bucket = render->m_sRenderQueue->Pop();
    while(bucket->size > 0) {
        printf("-------->%d\n", bucket->size);
        delete bucket;
        bucket = render->m_sRenderQueue->Pop();
    }
    if (bucket->size == 0) {
        delete bucket;
    }
    return NULL;
}
