#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"

SPlayer::SPlayer(const char *url){
    //demuxer = new Demuxer();
    demuxer = new Mp4Demuxer();
}

SPlayer::~SPlayer(){
    delete demuxer;
}

int SPlayer::play(){
    demuxer->open();
    return 0;
}

