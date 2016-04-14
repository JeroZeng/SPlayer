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
    RenderQueue *m_sRenderQueue;

public:
    Render();
    virtual ~Render();

    USING_BASE;

    virtual int Init(SWindow *win);
    int Start(RenderQueue *queue);
    virtual int Draw(SBucket *bucket);
    virtual int ClearScreen();
    virtual int Exit();

private:
    static void* Loop(void *arg);

};


#endif// _RENDER_H_
