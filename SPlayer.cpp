#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"

SPlayer::SPlayer(const char *url){
    //demuxer = new Demuxer();
    demuxer = new Mp4Demuxer();
}

SPlayer::~SPlayer(){
    delete demuxer;
}

void SPlayer::Init(){
    demuxer->Open();
}

int SPlayer::Play(){
    demuxer->Start();
    return 0;
}

