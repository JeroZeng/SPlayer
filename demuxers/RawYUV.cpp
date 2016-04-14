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
        printf("---------->Open YUV\n\n");
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
    m_iFrameSize = w * h * 3 / 2;
    //m_bLoop = true;
    return 0;
}

int RawYUV::GetOneFrame(SBucket *bucket) {
    bucket->size = m_iFrameSize;
    if (m_pFile != (FILE*)NULL) {
        m_iFrameSize = fread(bucket->data, 1, m_iFrameSize, m_pFile);
        if(m_bLoop && m_iFrameSize != m_iWidth * m_iHeight * 3/2) {
            fseek(m_pFile, 0, SEEK_SET);
            m_iFrameSize = m_iWidth * m_iHeight * 3/2;
            m_iFrameSize = fread(bucket->data, 1, m_iFrameSize, m_pFile);
        }
    }
    return m_iFrameSize;
}
