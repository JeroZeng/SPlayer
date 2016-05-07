#include "GLFW/glad/glad.h"
#include "PCRender.h"

static const float vertices[] =
{
    -1.0f, -1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f,
    1.0f,  1.0f, 1.0f, 0.0f
};

PCRender::PCRender(SPlayer *player) {
    if (player != NULL)
        m_sPlayer = player;
}

PCRender::~PCRender() {
    glfwMakeContextCurrent(m_sWindow);
    if (glIsTexture(m_textures[0])) {
        glDeleteTextures(3, m_textures);
    }
    if (glIsBuffer(m_vertexBuffer)) {
        glDeleteBuffers(1, &m_vertexBuffer);
    }
    if (glIsProgram(m_shaderProgram)) {
        glDeleteProgram(m_shaderProgram);
    }
    glfwMakeContextCurrent(NULL);
}


GLuint PCRender::MakeShader(GLenum type, const char* text)
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

GLuint PCRender::MakeShaderProgram(const char* vs_text, const char* fs_text)
{
    GLuint program = 0u;
    GLint program_ok;
    GLuint vertex_shader = 0u;
    GLuint fragment_shader = 0u;
    GLsizei log_length;
    char info_log[8192];

    vertex_shader = MakeShader(GL_VERTEX_SHADER, vs_text);
    if (vertex_shader != 0u)
    {
        fragment_shader = MakeShader(GL_FRAGMENT_SHADER, fs_text);
        if (fragment_shader != 0u)
        {
            /* make the program that connect the two shader and link it */
            program = glCreateProgram();
            if (program != 0u)
            {
                /* attach both shader and link */
                glAttachShader(program, vertex_shader);
                glAttachShader(program, fragment_shader);
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
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    return program;
}
int PCRender::Init(SWindow *win) {
    GLint vpos_loc, tbuf_loc;
    m_sWindow = win;

    if (m_iWidth * m_iHeight > 0) {
        glfwSetWindowSize(m_sWindow, m_iWidth, m_iHeight);
        GLFWvidmode *mode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(m_sWindow, (mode->width-m_iWidth)/2, (mode->height-m_iHeight)/2);
    }

    glfwMakeContextCurrent(m_sWindow);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    m_shaderProgram = MakeShaderProgram(vs, fs);
    if (m_shaderProgram == 0u) {
        return -1;
    }

    vpos_loc = glGetAttribLocation(m_shaderProgram , "vertexIn");
    glEnableVertexAttribArray(vpos_loc);
    glVertexAttribPointer(vpos_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) 0);

    tbuf_loc = glGetAttribLocation(m_shaderProgram , "textureIn");
    glEnableVertexAttribArray(tbuf_loc);
    glVertexAttribPointer(tbuf_loc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 4, (void*) (sizeof(float) * 2));

    //Init Texture
    glGenTextures(3, m_textures);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, m_textures[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glfwMakeContextCurrent(NULL);

    return 0;
}

int PCRender::Draw(SBucket *bucket) {
    glfwMakeContextCurrent(m_sWindow);
    glUseProgram(m_shaderProgram);

    int w, h;
    glfwGetFramebufferSize(m_sWindow, &w, &h);
    double rate = (double)w/m_iWidth>(double)h/m_iHeight?(double)h/m_iHeight:(double)w/m_iWidth;
    glViewport((w-m_iWidth*rate)/2, (h-m_iHeight*rate)/2, m_iWidth*rate, m_iHeight*rate);

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_iWidth, m_iHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bucket->data);
    GLuint textureUniformY = glGetUniformLocation(m_shaderProgram, "tex_y");
    glUniform1i(textureUniformY, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 m_iWidth / 2, m_iHeight / 2, 0, GL_RED,
                 GL_UNSIGNED_BYTE, bucket->data + m_iWidth * m_iHeight);
    GLuint textureUniformU = glGetUniformLocation(m_shaderProgram, "tex_u");
    glUniform1i(textureUniformU, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 m_iWidth / 2,
                 m_iHeight / 2, 0, GL_RED,
                 GL_UNSIGNED_BYTE,
                 bucket->data + (m_iWidth * m_iHeight * 5 / 4));
    GLuint textureUniformV = glGetUniformLocation(m_shaderProgram, "tex_v");
    glUniform1i(textureUniformV, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    while(glfwGetTime() < 1/m_dFPS);
    glfwSwapBuffers(m_sWindow);
    glfwSetTime(0.0);
    glfwMakeContextCurrent(NULL);

    return 0;
}

int PCRender::ClearScreen() {
    glfwMakeContextCurrent(m_sWindow);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_sWindow);
    glfwMakeContextCurrent(NULL);
    return 0;
}

int PCRender::WaitThreadClose() {
    //Stop();
    //Render::Stop();
    Render::WaitThreadClose();
    printf("-------------->Render Stoped------------\n");
    return 0;
}

int PCRender::Exit() {
    glfwTerminate();
    return 0;
}
