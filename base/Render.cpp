#include "Render.h"

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
    SData *data = render->m_sRenderQueue->Pop();
    while(data->num > 0) {
        printf("-------->%d\n", data->num);
        delete data;
        data = render->m_sRenderQueue->Pop();
    }
    if (data->num == 0) {
        delete data;
    }
    return NULL;
}
