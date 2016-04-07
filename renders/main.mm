//
//  main.m
//  GlutDemo
//
//  Created by ZengFeng on 3/31/16.
//  Copyright © 2016 RealNetworks. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "GL/glew.h"
#include <GLUT/GLUT.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

//set '1' to choose a type of file to play
#define LOAD_RGB24   0
#define LOAD_BGR24   0
#define LOAD_BGRA    0
#define LOAD_YUV420P 1

int screen_w=500,screen_h=500;
const int pixel_w = 320, pixel_h = 180;
//Bit per Pixel
#if LOAD_BGRA
const int bpp=32;
#elif LOAD_RGB24|LOAD_BGR24
const int bpp=24;
#elif LOAD_YUV420P
const int bpp=12;
#endif
//YUV file
FILE *fp = NULL;
unsigned char buffer[pixel_w*pixel_h*bpp/8];
unsigned char buffer_convert[pixel_w*pixel_h*3];

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
    if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
        // Loop
        fseek(fp, 0, SEEK_SET);
        fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
    }
    
    //Make picture full of window
    //Move to(-1.0,1.0)
    glRasterPos3f(-1.0f,1.0f,0);
    //Zoom, Flip
    glPixelZoom((float)screen_w/(float)pixel_w, -(float)screen_h/(float)pixel_h);
    
    
    
#if LOAD_BGRA
    glDrawPixels(pixel_w, pixel_h,GL_BGRA, GL_UNSIGNED_BYTE, buffer);
#elif LOAD_RGB24
    glDrawPixels(pixel_w, pixel_h,GL_RGB, GL_UNSIGNED_BYTE, buffer);
#elif LOAD_BGR24
    glDrawPixels(pixel_w, pixel_h,GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
#elif LOAD_YUV420P
    CONVERT_YUV420PtoRGB24(buffer,buffer_convert,pixel_w,pixel_h);
    glDrawPixels(pixel_w, pixel_h,GL_RGB, GL_UNSIGNED_BYTE, buffer_convert);
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



int main(int argc, char* argv[])
{
#if LOAD_BGRA
    fp=fopen("../test_bgra_320x180.rgb","rb+");
#elif LOAD_RGB24
    fp=fopen("../test_rgb24_320x180.rgb","rb+");
#elif LOAD_BGR24
    fp=fopen("../test_bgr24_320x180.rgb","rb+");
#elif LOAD_YUV420P
    fp=fopen("test_yuv420p_320x180.yuv","rb+");
#endif
    if(fp==NULL){
        printf("Cannot open this file.\n");
        return -1;
    }
    
    // GLUT init
    printf("%d\t%s\n", argc, argv[0]);
    glutInit(&argc, argv);
    printf("%d\t%s\n", argc, argv[0]);
    //Double, Use glutSwapBuffers() to show
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
    //Single, Use glFlush() to show
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screen_w, screen_h);
    glutCreateWindow("Simplest Video Play OpenGL");
    printf("Simplest Video Play OpenGL\n");
    printf("Lei Xiaohua\n");
    printf("http://blog.csdn.net/leixiaohua1020\n");
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    printf("%d\t%s\n", argc, argv[0]);
    
    glutDisplayFunc(&display);
    glutTimerFunc(40, timeFunc, 0); 
    
    // Start!
    glutMainLoop();
    
    return 0;
}
