#ifndef _H264_DECODER_H_
#define _H264_DECODER_H_

#include "../base/Decoder.h"
#include "openh264/codec_api.h"
#include "../SPlayer.h"
#define _DUMP_264_FILE_

class H264Decoder: public Decoder {

public:
    H264Decoder(SPlayer *p);
    ~H264Decoder();
    int Init();
    int DecodeOneFrame(SBucket *db, SBucket *rb);

private:
    SPlayer *player;
    ISVCDecoder *m_pDecoder;
    SDecodingParam m_sDecParam;
    SBufferInfo  m_sDstBufInfo;
#ifdef _DUMP_264_FILE_
    FILE *m_pDumpFile;
#endif
};

#endif//_H264_DECODER_H_
