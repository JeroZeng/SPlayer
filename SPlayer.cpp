#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"

SPlayer::SPlayer(const char *url){
    //demuxer = new Demuxer();
    demuxer = new Mp4Demuxer();
    decoder = new Decoder();
    render  = new Render();
}

SPlayer::~SPlayer(){
    delete demuxer;
    delete decoder;
    delete render;
}

void SPlayer::Init(){
    demuxer->Open("/dev/random");
}

int SPlayer::Play(){
    demuxer->Start();
    decoder->Start(demuxer->m_sQueue);
    render->Start(decoder->m_sRenderQueue);
    demuxer->WaitStreamEnd();
    decoder->WaitStreamEnd();
    render->WaitStreamEnd();
    return 0;
}

