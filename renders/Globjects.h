#include <stdlib.h>
#include "../Config.h"

#define _DEBUG_

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

#ifdef _DEBUG_
#include <stdio.h>
#define CheckErr(func) \
    if (glGetError() != 0) { \
        fprintf(m_pLog, "%s\t%d\t%d\n", __FILE__, glGetError(), __LINE__); \
    }
#define CheckErrEx(func) \
    if (glGetError() != 0) { \
        fprintf(m_glObjects.m_pLog, "%s\t%d\t%d\n", __FILE__, glGetError(), __LINE__); \
    }
#else
#define CheckErr(func)
#define CheckErrEx(func)
#endif

static const GLfloat g_flip[] =
{
    1,  0, 0, 0,
    0, -1, 0, 0,
    0,  0, 1, 0,
    0,  1, 0, 1
};

//static const char *vs =
//    "attribute vec4 vertexIn;"
//    "uniform mat4 uSTMatrix;"
//    "varying vec2 textureOut;"
//    "void main()"
//    "{"
//    "     gl_Position = vertexIn;"
//    "     gl_Position.xy = vec2(-1.0) + 2.0*vertexIn.xy;"
//    "     textureOut = (uSTMatrix * vertexIn).xy;"
//    "}";

static const char *vs =
    "attribute vec4 vertexIn;"
    "attribute vec2 textureIn;"
    "varying vec2 textureOut;"
    "void main()"
    "{"
    "     gl_Position = vertexIn;"
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
    "    rgb.r = yuv.x + (1.403 * yuv.z);"
    "    rgb.g = yuv.x - (0.344 * yuv.y) - (0.714 * yuv.z);"
    "    rgb.b = yuv.x + (1.770 * yuv.y);"
    "     gl_FragColor = vec4(rgb, 1);"
    "}";

class GLObjects
{
public:
    GLuint  m_textureY;
    GLuint  m_textureU;
    GLuint  m_textureV;
    GLuint  m_program;

#ifdef _DEBUG_
    FILE    *m_pLog;
#endif

public:
    GLObjects();
    ~GLObjects();

    int initTexture();
    int initShader();
    void setShaderArgs(unsigned char *yuv, int width, int height);
};
