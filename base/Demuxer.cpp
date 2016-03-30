#include <stdio.h>
#include "Demuxer.h"

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

int Demuxer::GetOneFrame(char *bucket){
    return 0;
}

void* Demuxer::Loop(void *arg){

    Demuxer *demuxer = (Demuxer*)arg;
    char *dt;
    int dt_size = demuxer->GetOneFrame(dt);
    while(dt_size > 0){
        SBucket *bucket = new SBucket();
        bucket->data = dt;
        bucket->size = dt_size;
        demuxer->m_sQueue->Push(bucket);
        dt_size = demuxer->GetOneFrame(dt);
    }
    SBucket *bucket = new SBucket();
    bucket->size = 0;
    demuxer->m_sQueue->Push(bucket);
    printf("------>END<------\n");
    return NULL;
}

int Demuxer::WaitStreamEnd(){

    pthread_join(m_pThreadDemuxer, NULL);
    return 0;
}

void Demuxer::SetFrameSize(int size) {
    m_iFrameSize = size;
}

int Demuxer::GetFrameSize() {
    return m_iFrameSize;
}
