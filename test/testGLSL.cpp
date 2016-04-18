#include <stdio.h>
#include <pthread.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4

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

static GLuint make_shader(GLenum type, const char* text)
{
    GLuint shader;
    GLint shader_ok;
    GLsizei log_length;
    char info_log[8192];

    shader = glCreateShader(type);
    if (shader != 0)
    {
        glShaderSource(shader, 1, (const GLchar**)&text, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
        if (shader_ok != GL_TRUE)
        {
            fprintf(stderr, "ERROR: Failed to compile %s shader\n", (type == GL_FRAGMENT_SHADER) ? "fragment" : "vertex");
            glGetShaderInfoLog(shader, 8192, &log_length, info_log);
            fprintf(stderr, "ERROR: \n%s\n\n", info_log);
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

static GLuint make_shader_program(const char* vs_text, const char* fs_text)
{
    GLuint program = 0u;
    GLint program_ok;
    GLuint vertex_shader = 0u;
    GLuint fragment_shader = 0u;
    GLsizei log_length;
    char info_log[8192];

    vertex_shader = make_shader(GL_VERTEX_SHADER, vs_text);
    if (vertex_shader != 0u)
    {
        fragment_shader = make_shader(GL_FRAGMENT_SHADER, fs_text);
        if (fragment_shader != 0u)
        {
            /* make the program that connect the two shader and link it */
            program = glCreateProgram();
            if (program != 0u)
            {
                /* attach both shader and link */
                glAttachShader(program, vertex_shader);
                glAttachShader(program, fragment_shader);
                glBindAttribLocation(program, ATTRIB_VERTEX, "vertexIn");
                glBindAttribLocation(program, ATTRIB_TEXTURE, "textureIn");
                glLinkProgram(program);
                glGetProgramiv(program, GL_LINK_STATUS, &program_ok);

                if (program_ok != GL_TRUE)
                {
                    fprintf(stderr, "ERROR, failed to link shader program\n");
                    glGetProgramInfoLog(program, 8192, &log_length, info_log);
                    fprintf(stderr, "ERROR: \n%s\n\n", info_log);
                    glDeleteProgram(program);
                    glDeleteShader(fragment_shader);
                    glDeleteShader(vertex_shader);
                    program = 0u;
                }
            }
        }
        else
        {
            fprintf(stderr, "ERROR: Unable to load fragment shader\n");
            glDeleteShader(vertex_shader);
        }
    }
    else
    {
        fprintf(stderr, "ERROR: Unable to load vertex shader\n");
    }
    return program;
}

GLenum error;
#define CheckErr(func) \
    error = glGetError(); \
    if (error != 0) { \
        printf("%s\t%x\t%d\n", __FILE__, error, __LINE__); \
        }

GLFWwindow* window;
bool g_stop = false;
GLuint shader_program;

GLuint  m_textureY;
GLuint  m_textureU;
GLuint  m_textureV;

const int width = 352, height = 288;
unsigned char yuv[width*height * 3 / 2];
FILE *fp = NULL;

#ifdef __APPLE__
void* ThreadFun(void *arg) {
#else
DWORD WINAPI ThreadFun(LPVOID pM) {
#endif
    glfwMakeContextCurrent(window);
    glfwSetTime(0.04);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    while (!g_stop)
    {
        if (glfwGetTime() < 0.04)
        {
            continue;
        }
        glUseProgram(shader_program);

        glClear(GL_COLOR_BUFFER_BIT);
        CheckErr(glUseProgram);

        if (width*height * 3 / 2 != fread(yuv, 1, width*height * 3 / 2, fp))
        {
            fseek(fp, 0, SEEK_SET);
        }

        static const GLfloat vertexVertices[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f,
        };
        static const GLfloat textureVertices[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
        };
        glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
        CheckErr(glVertexAttribPointer);
        //Enable it
        glEnableVertexAttribArray(ATTRIB_VERTEX);
        glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
        glEnableVertexAttribArray(ATTRIB_TEXTURE);

        glActiveTexture(GL_TEXTURE0);
        CheckErr(glActiveTexture);
        glBindTexture(GL_TEXTURE_2D, m_textureY);
        CheckErr(glTexImage2D);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, yuv);
        CheckErr(glTexImage2D);
        GLuint textureUniformY = glGetUniformLocation(shader_program, "tex_y");
        CheckErr(glGetUniformLocation);
        glUniform1i(textureUniformY, 0);
        CheckErr(glUniform1i);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureU);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            width / 2, height / 2, 0, GL_RED,
            GL_UNSIGNED_BYTE, yuv + width * height);
        GLuint textureUniformU = glGetUniformLocation(shader_program, "tex_u");
        CheckErr(glUniform1i);
        glUniform1i(textureUniformU, 1);
        CheckErr(glUniform1i);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textureV);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            width / 2,
            height / 2, 0, GL_RED,
            GL_UNSIGNED_BYTE,
            yuv + (width * height * 5 / 4));
        GLuint textureUniformV = glGetUniformLocation(shader_program, "tex_v");
        glUniform1i(textureUniformV, 2);
        CheckErr(glUniform1i);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        CheckErr(glDrawArrays);

        glfwSwapBuffers(window);
        glfwSetTime(0);
    }
    glfwMakeContextCurrent(NULL);
    return NULL;
}

int main(int argc, char** argv)
{
    fp = fopen("../flower_352_288.yuv", "rb");

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "GLFW OpenGL3 Heightmap demo", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    /* Prepare opengl resources for rendering */
    shader_program = make_shader_program(vs, fs);

    if (shader_program == 0u)
    {
        glfwTerminate();
        return -1;
    }

    //Init Texture
    glGenTextures(1, &m_textureY);
    glBindTexture(GL_TEXTURE_2D, m_textureY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &m_textureU);
    glBindTexture(GL_TEXTURE_2D, m_textureU);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &m_textureV);
    glBindTexture(GL_TEXTURE_2D, m_textureV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glfwMakeContextCurrent(NULL);

#ifdef __APPLE__
    pthread_t thr;
    pthread_create(&thr, NULL, ThreadFun, NULL);
#else
    CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL);
#endif

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
    g_stop = true;
    glfwTerminate();
    return 0;
}

