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

int Mp4Demuxer::Open(){
    fp = fopen("/dev/random", "r");
    if (fp) {
        printf("------>open mp4\n");
    }
    return 0;
}

int Mp4Demuxer::GetOneFrame(char *data){
    uint64_t value = 0;
    int i;
    for (i=0; i<sizeof(value); i++) {
        value <<= 8;
        value |= fgetc(fp);
    }
    i = value % 20;
    printf("%d<--------\n", i);
    return i;
}
