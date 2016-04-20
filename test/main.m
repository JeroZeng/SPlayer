#include <stdio.h>
#include <stdlib.h>
#include "glad.h"
#include "glfw3.h"

#define ATTRIB_VERTEX  8

static const float vertices[] =
{
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f,
    1.0f,  1.0f, 1.0f, 0.0f
};

static const char* vertex_shader_text =
"attribute vec2 vPos;"
"attribute vec2 textureIn;"
"varying vec2 textureOut;"
"void main()\n"
"{"
"    gl_Position =  vec4(vPos, 0.0, 1.0);"
"    textureOut = textureIn;"
"}";

static const char* fragment_shader_text =
"varying vec2 textureOut;"
"uniform sampler2D tex_y;"
"void main()"
"{"
"    vec3 yuv;"
"    vec3 rgb;"
"    yuv.x = texture2D(tex_y, textureOut).x;"
"    yuv.y = 0.0;"
"    yuv.z = 0.0;"
"    rgb.r = yuv.x + (1.403 * yuv.z);"
"    rgb.g = yuv.x - (0.344 * yuv.y) - (0.714 * yuv.z);"
"    rgb.b = yuv.x + (1.770 * yuv.y);"
"    gl_FragColor = vec4(rgb, 1.0);"
//"    gl_FragColor = vec4(yuv.x, yuv.x, yuv.x, 1.0);"
"}";

GLenum error;
#define CheckErr() \
error = glGetError(); \
if (error != 0) { \
printf("%s\t%x\t%d\n", __FILE__, error, __LINE__); \
}

int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint vpos_location, texBuf_loc;
    GLuint  m_textureY;
    unsigned char yuv[352*288 * 3 / 2];

    FILE *fp = fopen("./flower_352_288.yuv", "rb");
    fread(yuv, 1, 352*288 * 3 / 2, fp);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(352, 288, "SPlayer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    vpos_location = glGetAttribLocation(program, "vPos");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) 0);

    texBuf_loc = glGetAttribLocation(program, "textureIn");
    glEnableVertexAttribArray(texBuf_loc);
    glVertexAttribPointer(texBuf_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) (sizeof(float) * 2));

    //Init Texture
    glGenTextures(1, &m_textureY);
    glBindTexture(GL_TEXTURE_2D, m_textureY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0);
        CheckErr();
        glBindTexture(GL_TEXTURE_2D, m_textureY);
        CheckErr();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, yuv);
        CheckErr();
        GLuint textureUniformY = glGetUniformLocation(program, "tex_y");
        CheckErr();
        glUniform1i(textureUniformY, 0);
        CheckErr();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

