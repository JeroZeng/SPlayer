#ifndef _PC_RENDER_
#define _PC_RENDER_
#include "../base/render.h"

class PCRender : public Render {
public:
    ~PCRender();
    int Start(RenderQueue *queue);
    int WaitStreamEnd();

public:
    SBucket *m_sBucket;
    unsigned char *m_chConvertBuffer;
};

#endif//_PC_RENDER_
