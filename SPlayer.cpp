#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"

SPlayer::SPlayer(const char *url){
    //demuxer = new Demuxer();
    demuxer = new Mp4Demuxer();
    decoder = new Decoder();
}

SPlayer::~SPlayer(){
    delete demuxer;
    delete decoder;
}

void SPlayer::Init(){
    demuxer->Open();
}

int SPlayer::Play(){
    demuxer->Start();
    decoder->Start(demuxer->m_sQueue);
    demuxer->WaitThread();
    decoder->WaitThread();
    return 0;
}

