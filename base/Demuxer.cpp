#include <stdio.h>
#include "Demuxer.h"

Demuxer::~Demuxer(){
}

int Demuxer::Open(){

    return 0;
}

int Demuxer::Start(){

    m_sQueue = new SQueue(10);
    pthread_create(&m_pThreadDemuxer, NULL, Demuxer::Loop, (void*)this);

    return 0;
}

int Demuxer::GetOneFrame(char *data){
    return 0;
}

void* Demuxer::Loop(void *arg){

    Demuxer *demuxer = (Demuxer*)arg;
    char *dt;
    int dt_size = demuxer->GetOneFrame(dt);
    while(dt_size > 0){
        SData *data = new SData();
        data->size = dt_size;
        demuxer->m_sQueue->Push(data);
        dt_size = demuxer->GetOneFrame(dt);
    }
    SData *data = new SData();
    data->size = 0;
    demuxer->m_sQueue->Push(data);
    printf("------>END<------\n");
    return NULL;
}

int Demuxer::WaitStreamEnd(){

    pthread_join(m_pThreadDemuxer, NULL);
    return 0;
}
