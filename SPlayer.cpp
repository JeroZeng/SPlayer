#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"
#include "demuxers/RawYUV.h"
#include "renders/PCRender.h"

SPlayer::SPlayer(const char *url){
    //demuxer = new Demuxer();
    //demuxer = new Mp4Demuxer();
    demuxer = new RawYUV();
    decoder = new Decoder();
    //render  = new Render();
    render  = new PCRender();
}

SPlayer::~SPlayer(){
    delete demuxer;
    delete decoder;
    delete render;
}

void SPlayer::Init(){
    demuxer->Open("../flower_352_288.yuv");
    render->m_iWidth = decoder->m_iWidth = demuxer->m_iWidth;
    render->m_iHeight = decoder->m_iHeight = demuxer->m_iHeight;
}

int SPlayer::Play(){
    demuxer->Start();
    decoder->Start(demuxer->m_sQueue);
    render->Start(decoder->m_sRenderQueue);
    demuxer->WaitStreamEnd();
    decoder->WaitStreamEnd();
    render->WaitStreamEnd();
    demuxer->ClearMem();
    decoder->ClearMem();
    return 0;
}

