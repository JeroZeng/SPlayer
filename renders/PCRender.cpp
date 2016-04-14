#include "PCRender.h"

extern void keycallback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);

//set '1' to choose a type of file to play
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_BGRA    0
#define LOAD_YUV420P 1

#if LOAD_BGRA
const int bpp=32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp=24;
#elif LOAD_YUV420P
const int bpp=12;
#endif

inline unsigned char CONVERT_ADJUST(double tmp)
{
    return (unsigned char)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));
}

//YUV420P to RGB24
void CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight)
{
    unsigned char *tmpbuf=(unsigned char *)malloc(nWidth*nHeight*3);
    unsigned char Y,U,V,R,G,B;
    unsigned char* y_planar,*u_planar,*v_planar;
    int rgb_width , u_width;
    rgb_width = nWidth * 3;
    u_width = (nWidth >> 1);
    int ypSize = nWidth * nHeight;
    int upSize = (ypSize>>2);
    int offSet = 0;
    
    y_planar = yuv_src;
    u_planar = yuv_src + ypSize;
    v_planar = u_planar + upSize;
    
    for(int i = 0; i < nHeight; i++)
    {
        for(int j = 0; j < nWidth; j ++)
        {
            // Get the Y value from the y planar
            Y = *(y_planar + nWidth * i + j);
            // Get the V value from the u planar
            offSet = (i>>1) * (u_width) + (j>>1);
            V = *(u_planar + offSet);
            // Get the U value from the v planar
            U = *(v_planar + offSet);
            
            // Cacular the R,G,B values
            // Method 1
            R = CONVERT_ADJUST((Y + (1.4075 * (V - 128))));
            G = CONVERT_ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));
            B = CONVERT_ADJUST((Y + (1.7790 * (U - 128))));
            /*
             // The following formulas are from MicroSoft' MSDN
             int C,D,E;
             // Method 2
             C = Y - 16;
             D = U - 128;
             E = V - 128;
             R = CONVERT_ADJUST(( 298 * C + 409 * E + 128) >> 8);
             G = CONVERT_ADJUST(( 298 * C - 100 * D - 208 * E + 128) >> 8);
             B = CONVERT_ADJUST(( 298 * C + 516 * D + 128) >> 8);
             R = ((R - 128) * .6 + 128 )>255?255:(R - 128) * .6 + 128;
             G = ((G - 128) * .6 + 128 )>255?255:(G - 128) * .6 + 128; 
             B = ((B - 128) * .6 + 128 )>255?255:(B - 128) * .6 + 128; 
             */
            offSet = rgb_width * i + j * 3;
            
            rgb_dst[offSet] = B;
            rgb_dst[offSet + 1] = G;
            rgb_dst[offSet + 2] = R;
        }
    }
    free(tmpbuf);
}

PCRender::PCRender(SPlayer *player) {
    if (player != NULL)
        m_sPlayer = player;
}

PCRender::~PCRender() {
    if (m_chConvertBuffer != NULL) {
        free(m_chConvertBuffer);
        m_chConvertBuffer = NULL;
    }
}

int PCRender::Init() {
    if (!glfwInit())
        return -1;
    m_glfwWindow = glfwCreateWindow(m_iWidth, m_iHeight, "SPlayer", NULL, NULL);
    glfwSetWindowUserPointer(m_glfwWindow, m_sPlayer);
    glfwSetKeyCallback(m_glfwWindow, keycallback);
    //glfwSetWindowPos(m_glfwWindow, 300, 200);
    glfwShowWindow(m_glfwWindow);
    if (!m_glfwWindow) {
        printf("Create Window Failed\n");
        glfwTerminate();
        return -1;
    }
    //glfwMakeContextCurrent(m_glfwWindow);

    m_chConvertBuffer = (unsigned char*)malloc(m_iWidth * m_iHeight * sizeof(unsigned char)*3);

    return 0;
}

int PCRender::Draw(SBucket *bucket) {
    glfwMakeContextCurrent(m_glfwWindow);
    glRasterPos3f(-1.0f,1.0f,0);
    glPixelZoom((float)m_iWidth/(float)m_iWidth, -(float)m_iHeight/(float)m_iHeight);
    CONVERT_YUV420PtoRGB24((unsigned char*)bucket->data,m_chConvertBuffer,m_iWidth,m_iHeight);
    glDrawPixels(m_iWidth, m_iHeight,GL_RGB, GL_UNSIGNED_BYTE, m_chConvertBuffer);
    glfwSwapBuffers(m_glfwWindow);
    glfwMakeContextCurrent(NULL);
    return 0;
}

int PCRender::ClearScreen() {
    glfwMakeContextCurrent(m_glfwWindow);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_glfwWindow);
    glfwMakeContextCurrent(NULL);
    return 0;
}

int PCRender::WaitThreadClose() {
    while (!glfwWindowShouldClose(m_glfwWindow)) {
        glfwWaitEvents();    
    }
    Stop();
    glfwHideWindow(m_glfwWindow);
    //Render::Stop();
    Render::WaitThreadClose();
    printf("-------------->Render Stoped------------\n");
    return 0;
}

int PCRender::Exit() {
    glfwTerminate();
    return 0;
}
