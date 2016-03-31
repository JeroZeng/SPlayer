#include <stdio.h>
#include "Demuxer.h"

#define BUFFER8  8 * 1024 * 1024

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
#ifdef _DEBUG_
    int frame_num = 0;
#endif//_Debug_
    int iMemBarUsed = 0;
    memBar[i] = (char*)MALLOC(BUFFER8 * sizeof(char));
    SBucket *bucket = new SBucket;
    bucket->data = memBar[i];
    int nextFrameSize = demuxer->GetOneFrame(bucket);
    iMemBarUsed += bucket->size;
    while(nextFrameSize > 0){
        //printf("------>i_size: %d\t<------\n", bucket->size);
        demuxer->m_sQueue->Push(&bucket);
        if (bucket->data == memBar[0]) {
            i = 0;
            iMemBarUsed = 0;
        }
        bucket->data = memBar[i]+iMemBarUsed;
        if (iMemBarUsed + nextFrameSize > BUFFER8) {
            i++;
            if (memBar[i] == NULL) {
                memBar[i] = (char*)MALLOC(BUFFER8 * sizeof(char));
            }
            bucket->data = memBar[i];
            iMemBarUsed = 0;
        }
        nextFrameSize = demuxer->GetOneFrame(bucket);
        iMemBarUsed += bucket->size;
#ifdef _DEBUG_
        frame_num++;
#endif//_DEBUG_
    }
    bucket->size = 0;
    demuxer->m_sQueue->Push(&bucket);
#ifdef _DEBUG_
    printf("------>Frame\t%d<------\n", frame_num);
#endif//_DEBUG_
    delete bucket;
    return NULL;
}

int Demuxer::WaitStreamEnd(){

    pthread_join(m_pThreadDemuxer, NULL);
    return 0;
}
