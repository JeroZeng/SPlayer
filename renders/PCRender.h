#ifndef _PC_RENDER_
#define _PC_RENDER_
#include "../SPlayer.h"
#include "../base/render.h"
#include "Painter.h"

class PCRender : public Render {
public:
    PCRender(SPlayer* player);
    ~PCRender();
    int Init(SWindow *win);
    int Draw(SBucket *bucket);
    int ClearScreen();
    int WaitThreadClose();
    bool ShouldExit();
    int Exit();

public:
    SPlayer *m_sPlayer;
    Painter *m_sPainter;
    unsigned char *m_chConvertBuffer;

private:
    SWindow *m_sWindow; 
};

#endif//_PC_RENDER_
