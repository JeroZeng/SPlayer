#include <stdio.h>
#include "Mp4Demuxer.h"

int Mp4Demuxer::Open(){
    printf("------>open mp4\n");
    return 0;
}

int Mp4Demuxer::GetOneFrame(char *data){
    return 1;
}
