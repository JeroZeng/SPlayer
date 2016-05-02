#include "H264Decoder.h"

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
    unsigned char* dst = NULL;
    printf("------------------->DecodeOne Frame: %d\n", db->size);
    m_pDecoder->DecodeFrameNoDelay(db->data, db->size, &dst, &m_sDstBufInfo);
    if (m_sDstBufInfo.iBufferStatus == 1) {
        printf("------------------->Decode One Frame\n");
        if (rb->data == NULL) {
            m_iWidth = m_sDstBufInfo.UsrData.sSystemBuffer.iWidth;
            m_iHeight = m_sDstBufInfo.UsrData.sSystemBuffer.iHeight;
            rb->size = m_iWidth * m_iHeight;
            rb->data = (unsigned char*)malloc(rb->size*sizeof(unsigned char));
        }
        memcpy(rb->data, dst, rb->size);
        return RES_OK;
    }
    return 1;
}
