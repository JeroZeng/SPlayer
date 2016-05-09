#ifndef _RENDER_H_
#define _RENDER_H_

#include "Base.h"
#include "Queue.h"
#include "../Config.h"

class RenderQueue : public SQueue {
public:
    RenderQueue(int len);

};

class Render : private Base {
public:
    int m_iWidth;
    int m_iHeight;
    double m_dFPS;
    RenderQueue *m_sRenderQueue;

public:
    Render();
    virtual ~Render();

    USING_BASE;

    virtual int Init(SWindow *win);
    virtual void SetFPS(double fps);
    int Start(RenderQueue *queue);
    virtual int Draw(SBucket *bucket);
    virtual int ClearScreen();
    virtual int Exit();

private:
#ifdef _WIN32
    static unsigned WINAPI Loop(void *arg);
#else
    static void* Loop(void *arg);
#endif

};


#endif// _RENDER_H_
