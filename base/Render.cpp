#include "Render.h"

RenderQueue::RenderQueue(int len):
    SQueue(len) {
}

Render::Render() {

}

Render::~Render() {
    m_dFPS = 25;
}

int Render::Init(SWindow *win) {
    return 0;
}

void Render::SetFPS(double fps) {
    m_dFPS = fps;
}

int Render::Start(RenderQueue *queue) {

    m_sRenderQueue = queue;
#ifdef _WIN32
    m_thread = (HANDLE)_beginthreadex(NULL, 0, Render::Loop, (void*)this, 0, NULL);
#else
    pthread_create(&m_thread, NULL, Render::Loop, (void*)this);
#endif
    return 0;
}

#ifdef _WIN32
unsigned WINAPI Render::Loop(void *arg) {
#else
void* Render::Loop(void *arg){
#endif
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
#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
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
