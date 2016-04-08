#include "PCRender.h"
#include "GL/glew.h"
#include <GLUT/GLUT.h>

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

PCRender *g_sRender = NULL;

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

void display(void){
    
    g_sRender->m_sRenderQueue->Pop(&g_sRender->m_sBucket);
    
    //Make picture full of window
    //Move to(-1.0,1.0)
    glRasterPos3f(-1.0f,1.0f,0);
    //Zoom, Flip
    glPixelZoom((float)g_sRender->m_iWidth/(float)g_sRender->m_iWidth, -(float)g_sRender->m_iHeight/(float)g_sRender->m_iHeight);
    
    
    
#if LOAD_BGRA
    glDrawPixels(g_sRender->m_iWight, g_sRender->m_iHeight,GL_BGRA, GL_UNSIGNED_BYTE, bucket->data);
#elif LOAD_RGB24
    glDrawPixels(g_sRender->m_iWight, g_sRender->m_iHeight,GL_RGB, GL_UNSIGNED_BYTE, bucket->data);
#elif LOAD_BGR24
    glDrawPixels(g_sRender->m_iWight, g_sRender->m_iHeight,GL_BGR_EXT, GL_UNSIGNED_BYTE, bucket->data);
#elif LOAD_YUV420P
    CONVERT_YUV420PtoRGB24((unsigned char*)g_sRender->m_sBucket->data,g_sRender->m_chConvertBuffer,g_sRender->m_iWidth,g_sRender->m_iHeight);
    glDrawPixels(g_sRender->m_iWidth, g_sRender->m_iHeight,GL_RGB, GL_UNSIGNED_BYTE, g_sRender->m_chConvertBuffer);
#endif
    //GLUT_DOUBLE
    glutSwapBuffers();
    
    //GLUT_SINGLE
    //glFlush();
}

void timeFunc(int value){
    display();
    // Present frame every 40 ms
    glutTimerFunc(40, timeFunc, 0);
}

PCRender::~PCRender() {
    if (m_chConvertBuffer != NULL) {
        free(m_chConvertBuffer);
        m_chConvertBuffer = NULL;
    }
    if (m_sBucket != NULL) {
        delete m_sBucket;
        m_sBucket = NULL;
    }
}

int PCRender::Start(RenderQueue *queue) {
    m_sRenderQueue = queue;
    m_chConvertBuffer = (unsigned char*)malloc(m_iWidth * m_iHeight * sizeof(unsigned char)*3);
    m_sBucket = new SBucket();
    g_sRender = this;
    int argc = 1;
    char *argv[1] = {"../playDemo"};
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(m_iWidth, m_iHeight);
    glutCreateWindow("SPlay");

    glutDisplayFunc(&display);
    glutTimerFunc(40, timeFunc, 0);

    glutMainLoop();

    return 0;
}

int PCRender::WaitStreamEnd() {
    return 0;
}
