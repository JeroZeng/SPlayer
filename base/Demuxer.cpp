#include <stdio.h>
#include "Demuxer.h"

#define BUFFER8 8 * 1024 * 1024

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
    char *memBar[5] = {NULL, NULL, NULL, NULL, NULL};
    int i = 0;
    int iMemBarUsed = 0;
    memBar[i] = (char*)MALLOC(BUFFER8 * sizeof(char));
    SBucket *bucket = new SBucket;
    bucket->data = memBar[i];
    int nextFrameSize = demuxer->GetOneFrame(bucket);
    iMemBarUsed += nextFrameSize;
    while(nextFrameSize > 0){
        demuxer->m_sQueue->Push(&bucket);
        if (bucket->data == memBar[0]) {
            i = 0;
            nextFrameSize = nextFrameSize;
        }
        if (iMemBarUsed > BUFFER8) {
            i++;
            if (memBar[i] == NULL) {
                memBar[i] = (char*)MALLOC(BUFFER8 * sizeof(char));
            }
            bucket->data = memBar[i];
            iMemBarUsed = nextFrameSize;
        }
        nextFrameSize = demuxer->GetOneFrame(bucket);
        iMemBarUsed += nextFrameSize;
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
