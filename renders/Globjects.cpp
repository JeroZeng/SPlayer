#include "globjects.h"

GLObjects::GLObjects()
{
#ifdef _DEBUG_
    m_pLog = fopen("/Users/Jero/Git/SPlayer/test/log", "a");
#endif
}

GLObjects::~GLObjects()
{
#ifdef _DEBUG_
    fclose(m_pLog);
#endif
}

int GLObjects::initTexture()
{
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

    return 0;
}

int GLObjects::initShader()
{
    GLint vShader, fShader;
    GLint status = 0;
    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShader, 1, &vs, NULL);
    glShaderSource(fShader, 1, &fs, NULL);

    glCompileShader(vShader);
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &status);
    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &status);

    m_program = glCreateProgram();
    CheckErr(glCreateProgram);

    glAttachShader(m_program, vShader);
    glAttachShader(m_program, fShader);

    glBindAttribLocation(m_program, ATTRIB_VERTEX, "vertexIn");
    glBindAttribLocation(m_program, ATTRIB_TEXTURE, "textureIn");

    glLinkProgram(m_program);
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    CheckErr(glLinkProgram);

    return 0;
}

void GLObjects::setShaderArgs(unsigned char *yuv, int width, int height)
{
    //unsigned char vertices[] = {0, 0, 1, 0, 0, 1, 1, 1};
    //GLint positionHandle = glGetAttribLocation(m_program, "vertexIn");
    //glVertexAttribPointer(positionHandle, 2, GL_BYTE, false, 0, vertices);
    //glEnableVertexAttribArray(positionHandle);

    //GLint matrixHandle = glGetUniformLocation(m_program, "uSTMatrix");
    //glUniformMatrix4fv(matrixHandle, 1, false, g_flip);
    //CheckErr(glUniformMatrix4fv);

    //Set Arrays
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
    //Enable it
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, yuv);
    GLuint textureUniformY = glGetUniformLocation(m_program, "tex_y");
    glUniform1i(textureUniformY, 0);
    CheckErr(glUniform1i);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureU);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        width / 2, height / 2, 0, GL_RED,
        GL_UNSIGNED_BYTE, yuv + width * height);
    GLuint textureUniformU = glGetUniformLocation(m_program, "tex_u");
    glUniform1i(textureUniformU, 1);
    CheckErr(glUniform1i);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_textureV);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        width / 2,
        height / 2, 0, GL_RED,
        GL_UNSIGNED_BYTE,
        yuv + (width * height * 5 / 4));
    GLuint textureUniformV = glGetUniformLocation(m_program, "tex_v");
    glUniform1i(textureUniformV, 2);
    CheckErr(glUniform1i);
}
