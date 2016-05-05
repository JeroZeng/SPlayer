#ifndef _PC_RENDER_
#define _PC_RENDER_
#include "../SPlayer.h"
#include "../base/render.h"

static const char *vs =
    "attribute vec2 vertexIn;"
    "attribute vec2 textureIn;"
    "varying vec2 textureOut;"
    "void main()"
    "{"
    "     gl_Position = vec4(vertexIn, 0.0, 1.0);;"
    "     textureOut = textureIn;"
    "}";

static const char *fs =
    "varying vec2 textureOut;"
    "uniform sampler2D tex_y;"
    "uniform sampler2D tex_u;"
    "uniform sampler2D tex_v;"
    "void main(void)"
    "{"
    "    vec3 yuv;"
    "    vec3 rgb;"
    "    yuv.x = texture2D(tex_y, textureOut).x;"
    "    yuv.y = texture2D(tex_u, textureOut).r - 0.5;"
    "    yuv.z = texture2D(tex_v, textureOut).r - 0.5;"
    //"    yuv.y = 0.0;"
    //"    yuv.z = 0.0;"
    "    rgb.r = yuv.x + (1.403 * yuv.z);"
    "    rgb.g = yuv.x - (0.344 * yuv.y) - (0.714 * yuv.z);"
    "    rgb.b = yuv.x + (1.770 * yuv.y);"
    "     gl_FragColor = vec4(rgb, 1);"
    "}";

class PCRender : public Render {
public:
    PCRender(SPlayer* player);
    ~PCRender();
    int Init(SWindow *win);
    GLuint MakeShader(GLenum type, const char *ch);
    GLuint MakeShaderProgram(const char *vs, const char *fs);
    int Draw(SBucket *bucket);
    int ResetClock();
    int ClearScreen();
    int WaitThreadClose();
    bool ShouldExit();
    int Exit();

public:
    SPlayer *m_sPlayer;

private:
    SWindow *m_sWindow; 
    GLuint m_shaderProgram;
    GLuint m_vertexBuffer;
    GLuint m_textures[3];
};

#endif//_PC_RENDER_
