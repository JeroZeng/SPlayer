#include <stdio.h>
#include "demuxer.h"

Demuxer::~Demuxer(){
}

int Demuxer::open(){

    printf("---> open file.\n");
    return 0;
}

