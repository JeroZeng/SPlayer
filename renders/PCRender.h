#ifndef _PC_RENDER_
#define _PC_RENDER_
#include "../base/render.h"
#include "GLFW/glfw3.h"

class PCRender : public Render {
public:
    ~PCRender();
    int Init();
    int Draw(SBucket *bucket);
    int ClearScreen();
    int WaitStreamEnd();
    bool ShouldExit();
    int Exit();

public:
    SBucket *m_sBucket;
    unsigned char *m_chConvertBuffer;

private:
    GLFWwindow *m_glfwWindow; 
    bool m_bExit;
};

#endif//_PC_RENDER_
