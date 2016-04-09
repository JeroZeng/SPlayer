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
    virtual int Init();
    int Start(RenderQueue *queue);
    virtual int Draw(SBucket *bucket);
    int Stop();
    virtual bool ShouldExit();
    virtual int Exit();
    virtual int WaitStreamEnd();

private:
    static void* Loop(void *arg);

private:
    pthread_t m_pThreadRender;

};


#endif// _RENDER_H_
