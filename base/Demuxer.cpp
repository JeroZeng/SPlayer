#include <stdio.h>
#include "Demuxer.h"

#define BUFFER_8M 8 * 1024 * 1024
#define MALLOC valloc

Demuxer::~Demuxer(){
}

int Demuxer::Open(const char *url){

    return 0;
}

int Demuxer::Start(){

    m_sQueue = new SQueue(10);
    pthread_create(&m_pThreadDemuxer, NULL, Demuxer::Loop, (void*)this);

    return 0;
}

int Demuxer::GetOneFrame(SBucket *bucket){
    return 0;
}

void* Demuxer::Loop(void *arg){

    Demuxer *demuxer = (Demuxer*)arg;
    char *data[8];
    int i = 0;
    data[i] = (char*)MALLOC(BUFFER_8M * sizeof(char));
    SBucket *bucket = new SBucket;
    bucket->data = data[i];
    int nextFrameSize = demuxer->GetOneFrame(bucket);
    while(nextFrameSize > 0){
        demuxer->m_sQueue->Push(&bucket);
        nextFrameSize = demuxer->GetOneFrame(bucket);
    }
    bucket->size = 0;
    demuxer->m_sQueue->Push(&bucket);
    printf("------>Video END<------\n");
    delete bucket;
    return NULL;
}

int Demuxer::WaitStreamEnd(){

    pthread_join(m_pThreadDemuxer, NULL);
    return 0;
}
