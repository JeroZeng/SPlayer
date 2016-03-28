#include <stdio.h>
#include "Demuxer.h"

Demuxer::~Demuxer(){
}

int Demuxer::Open(){

    return 0;
}

int Demuxer::Start(){

    m_sQueue = new SQueue();
    pthread_create(&m_pThreadDemuxer, NULL, Demuxer::Loop, (void*)this);

    return 0;
}

int Demuxer::GetOneFrame(char *data){
    return 0;
}

void* Demuxer::Loop(void *arg){

    Demuxer *demuxer = (Demuxer*)arg;
    char *dt;
    int dt_size = 0;
    while((dt_size = demuxer->GetOneFrame(dt)) > 0){
        SData *data = new SData();
        data->num = dt_size;
        demuxer->m_sQueue->Push(data);
    }
    return NULL;
}

int Demuxer::WaitThread(){

    pthread_join(m_pThreadDemuxer, NULL);
    return 0;
}
