#ifndef _RENDER_H_
#define _RENDER_H_

#include "Queue.h"

class RenderQueue : public SQueue {
public:
    RenderQueue(int len);

};

class Render{

public:
    Render();
    ~Render();
    void Init();
    int Start(RenderQueue *queue);
    int WaitStreamEnd();
    virtual int Draw(char *yuv, int width, int height);

private:
    static void* Loop(void *arg);

private:
    RenderQueue *m_sRenderQueue;
    pthread_t m_pThreadRender;

};


#endif// _RENDER_H_
