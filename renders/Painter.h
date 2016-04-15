#include "globjects.h"

class Painter
{
public:
    int m_iWidth;
    int m_iHeight;

    GLObjects m_glObjects;

public:
    Painter(int w, int h);
    ~Painter();
    int initGL(SWindow *win);
    void releaseGL();
    int draw(unsigned char *yuv);

private:
    bool    m_bGLInited;
    SWindow *m_sWindow;
};
