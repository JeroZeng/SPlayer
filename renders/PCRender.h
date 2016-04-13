#ifndef _PC_RENDER_
#define _PC_RENDER_
#include "../SPlayer.h"
#include "../base/render.h"
#include "GLFW/glfw3.h"

class PCRender : public Render {
public:
    PCRender(SPlayer* player);
    ~PCRender();
    int Init();
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    int Draw(SBucket *bucket);
    int ClearScreen();
    int WaitThreadClose();
    bool ShouldExit();
    int Exit();

public:
    SBucket *m_sBucket;
    SPlayer *m_sPlayer;
    unsigned char *m_chConvertBuffer;

private:
    GLFWwindow *m_glfwWindow; 
};

#endif//_PC_RENDER_
