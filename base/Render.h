#ifndef _RENDER_H_
#define _RENDER_H_

#include "Queue.h"

class RenderQueue : public SQueue {
public:
    RenderQueue(int len);

};

class Render{
public:
    int m_iWidth;
    int m_iHeight;
    RenderQueue *m_sRenderQueue;

public:
    Render();
    virtual ~Render();
    void Init();
    virtual int Start(RenderQueue *queue);
    virtual int WaitStreamEnd();
    virtual int Draw(char *yuv, int width, int height);

private:
    static void* Loop(void *arg);

private:
    pthread_t m_pThreadRender;

};


#endif// _RENDER_H_
