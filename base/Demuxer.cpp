#include <stdio.h>
#include "Demuxer.h"

#define BUFFER8  2 * 1024 * 1024

Demuxer::Demuxer():
    m_dFPS(25) {
    for (int i=0; i<5; i++) {
        m_MemBar[0][i] = NULL;
        m_MemBar[1][i] = NULL;
    }
}

Demuxer::~Demuxer(){
}

int Demuxer::Open(const char *url){

    return 0;
}

int Demuxer::Start(){

    m_sQueue = new SQueue(10);
    pthread_create(&m_thread, NULL, Demuxer::Loop, (void*)this);

    return 0;
}

int Demuxer::GetOneFrame(SBucket *bucket){
    return 0;
}

void* Demuxer::Loop(void *arg) {
    Demuxer *demuxer = (Demuxer*)arg;
    int i = 0; bool flag = 0;
#ifdef _DEBUG_
    int frame_num = 0;
#endif//_Debug_
    int iMemBarUsed = 0;
    demuxer->m_MemBar[0][i] = (unsigned char*)MALLOC(BUFFER8*sizeof(unsigned char));
    demuxer->m_MemBar[1][i] = (unsigned char*)MALLOC(BUFFER8*sizeof(unsigned char));
    SBucket *bucket = new SBucket;
    bucket->data = demuxer->m_MemBar[flag][i];
    int nextFrameSize = demuxer->GetOneFrame(bucket);
#ifdef _DEBUG_
    frame_num++;
#endif//_Debug_
    iMemBarUsed += bucket->size;
    while(nextFrameSize > 0){
        //printf("------>i_size: %d\t<------\n", bucket->size);
        if (demuxer->m_sQueue->Push(&bucket)) {
            break;
        }
        if (bucket->data == demuxer->m_MemBar[flag][0]) {
            i = 0;
            flag = !flag;
            iMemBarUsed = 0;
        }
        bucket->data = demuxer->m_MemBar[flag][i]+iMemBarUsed;
        if (iMemBarUsed + nextFrameSize > BUFFER8) {
            i++;
            if (demuxer->m_MemBar[flag][i] == NULL) {
                demuxer->m_MemBar[flag][i] = (unsigned char*)MALLOC(BUFFER8*sizeof(unsigned char));
            }
            bucket->data = demuxer->m_MemBar[flag][i];
            iMemBarUsed = 0;
        }
        nextFrameSize = demuxer->GetOneFrame(bucket);
        iMemBarUsed += bucket->size;
#ifdef _DEBUG_
        frame_num++;
#endif//_DEBUG_
    }
    demuxer->m_sQueue->Push(&bucket);
    if (nextFrameSize == 0) {
        bucket->size = 0;
        demuxer->m_sQueue->Push(&bucket);
    }
    demuxer->Reset();
#ifdef _DEBUG_
    printf("---------->Input %d Frames \n", frame_num);
#endif//_DEBUG_
    delete bucket;
    return NULL;
}

void Demuxer::ClearMem() {
    if (m_sQueue == NULL)
        return;
    delete m_sQueue;
    m_sQueue = NULL;
    for (int i=0; i<5; i++) {
        if (m_MemBar[0][i] != NULL) {
            free(m_MemBar[0][i]);
            m_MemBar[0][i] = NULL;
        }
        if (m_MemBar[1][i] != NULL) {
            free(m_MemBar[1][i]);
            m_MemBar[1][i] = NULL;
        }
    }
}

int Demuxer::Stop() {
    return pthread_kill(m_thread, SIGQUIT);
}

void Demuxer::Reset() {
}
