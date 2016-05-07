#include "H264Decoder.h"

H264Decoder::H264Decoder(SPlayer *p){
    player = p;
}

H264Decoder::~H264Decoder() {
    if (m_pDecoder) {
        m_pDecoder->Uninitialize();
        WelsDestroyDecoder(m_pDecoder);
    }
}

int H264Decoder::Init() {
    m_sDecParam.sVideoProperty.size = sizeof (m_sDecParam.sVideoProperty);
    m_sDecParam.uiTargetDqLayer = (uint8_t) - 1;
    m_sDecParam.eEcActiveIdc = ERROR_CON_SLICE_COPY;
    m_sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;

    WelsCreateDecoder(&m_pDecoder);
    if (NULL == m_pDecoder) {
        printf("Create Decoder Failed.\n");
        return 1;
    }
    if (m_pDecoder->Initialize(&m_sDecParam)) {
        printf("Decoder Initialization Failed.\n");
        return 1;
    }
    return RES_OK;
}

int H264Decoder::DecodeOneFrame(SBucket *db, SBucket *rb) {
    unsigned char *dst[3] = {NULL, NULL, NULL};
    unsigned char *ptr = NULL;
    int iStride[2];
    m_pDecoder->DecodeFrameNoDelay(db->data, db->size, dst, &m_sDstBufInfo);
    //printf("------------------->Decode Frame: %d\n", db->size);
    if (m_sDstBufInfo.iBufferStatus == 1) {
        if (rb->data == NULL) {
            m_iWidth = m_sDstBufInfo.UsrData.sSystemBuffer.iWidth;
            m_iHeight = m_sDstBufInfo.UsrData.sSystemBuffer.iHeight;
            player->render->m_iWidth = player->demuxer->m_iWidth = m_iWidth;
            player->render->m_iHeight = player->demuxer->m_iHeight = m_iHeight;
            rb->size = m_iWidth * m_iHeight * 3/2;
            rb->data = (unsigned char*)malloc(rb->size*sizeof(unsigned char));
        }
        iStride[0] = m_sDstBufInfo.UsrData.sSystemBuffer.iStride[0];
        iStride[1] = m_sDstBufInfo.UsrData.sSystemBuffer.iStride[1];
        rb->size = m_iWidth * m_iHeight *3/2;
        int frameSize = m_iWidth * m_iHeight;
        
        ptr = dst[0];
        for (int i=0; i<m_iHeight; i++) {
            memcpy(rb->data+m_iWidth*i, ptr+i*iStride[0], m_iWidth);
        }
        ptr = dst[1];
        for (int i=0; i<m_iHeight/2; i++) {
            memcpy(rb->data+frameSize+m_iWidth/2*i, ptr+i*iStride[1], m_iWidth/2);
        }
        ptr = dst[2];
        for (int i=0; i<m_iHeight/2; i++) {
            memcpy(rb->data+frameSize*5/4+m_iWidth/2*i, ptr+i*iStride[1], m_iWidth/2);
        }

        return RES_OK;
    }
    return 1;
}
