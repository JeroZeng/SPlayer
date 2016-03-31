#include <iostream>
#include "RawYUV.h"

RawYUV::RawYUV() {
}

RawYUV::~RawYUV() {

    if (m_pFile != (FILE*)NULL) {
        fclose(m_pFile);
    }

}

int RawYUV::Open(const char *url) {
    m_pFile = fopen(url, "r");
    if (m_pFile != (FILE*)NULL) {
        printf("------Open YUV--------\n");
        SetVideoProperty();
    }
    return 0;
}

int RawYUV::SetVideoProperty() {
    int w, h;
#if 0
    std::cout<<"Width: ";
    std::cin>>w;
    std::cout<<"Height: ";
    std::cin>>h;
    std::cout<<std::endl;
#else
    w = 352, h = 288;
#endif
    m_iWidth = w;
    m_iHeight = h;
    m_iFrameSize = w * h;
    return 0;
}

int RawYUV::GetOneFrame(SBucket *bucket) {
    std::cout<<"------FrameSize:"<<m_iFrameSize<<std::endl;;
    if (m_pFile != (FILE*)NULL) {
        m_iFrameSize = fread(bucket->data, 1, m_iFrameSize, m_pFile);
    }
    std::cout<<"------FrameSize:"<<m_iFrameSize<<std::endl;;
    return m_iFrameSize;
}
