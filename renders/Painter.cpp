#include "painter.h"
#include <stdio.h>

Painter::Painter(int w, int h)
: m_iWidth(w)
, m_iHeight(h)
, m_bGLInited(false)
{
}

Painter::~Painter()
{
    releaseGL();
}

int Painter::initGL(SWindow *win)
{
    if (m_bGLInited)
    {
        return 0;
    }

    if (win != NULL) {
        m_sWindow = win;
    }

    m_glObjects.initShader();
    m_glObjects.initTexture();
    m_bGLInited = true;

    return 0;
}

void Painter::releaseGL()
{
    glfwMakeContextCurrent(m_sWindow);
    //release textures
    if (glIsTexture(m_glObjects.m_textureY))
    {
        glDeleteTextures(1, &m_glObjects.m_textureY);
    }
    if (glIsTexture(m_glObjects.m_textureU))
    {
        glDeleteTextures(1, &m_glObjects.m_textureU);
    }
    if (glIsTexture(m_glObjects.m_textureV))
    {
        glDeleteTextures(1, &m_glObjects.m_textureV);
    }
    //release program
    if (glIsProgram(m_glObjects.m_program))
    {
        glDeleteProgram(m_glObjects.m_program);
    }
    glfwMakeContextCurrent(NULL);
}

int Painter::draw(unsigned char *yuv)
{
    if (m_sWindow == NULL)
    {
        return -1;
    }
    printf("---------------->Draw YUV<----------------\n");

    glfwMakeContextCurrent(m_sWindow);

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_glObjects.m_program);
    CheckErrEx("glUseProgram");

    m_glObjects.setShaderArgs(yuv, m_iWidth, m_iHeight);

    //glViewport(0, 0, m_iWidth, m_iHeight);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CheckErrEx("glDrawArrays");

    glfwSwapBuffers(m_sWindow);

    glFinish();

    //glFlush();

    glfwMakeContextCurrent(m_sWindow);

    return 0;
}
