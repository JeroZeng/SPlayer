#include <stdio.h>
#include <stdlib.h>
#include "Mp4Demuxer.h"

Mp4Demuxer::Mp4Demuxer(){
}

Mp4Demuxer::~Mp4Demuxer(){
    if (!fp) {
        fclose(fp);
    }
}

int Mp4Demuxer::Open(const char *url){
    fp = fopen(url, "r");
    if (fp) {
        printf("------>open mp4\n");
    }
    return 0;
}

int Mp4Demuxer::GetOneFrame(SBucket *bucket){
    return 0;
}
