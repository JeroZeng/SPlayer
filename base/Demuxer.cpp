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
#ifdef _WIN32
    m_thread = (HANDLE)_beginthreadex(NULL, 0, Demuxer::Loop, (void*)this, 0, NULL);
#else
    pthread_create(&m_thread, NULL, Demuxer::Loop, (void*)this);
#endif
    return 0;
}

int Demuxer::GetOneFrame(SBucket *bucket){
    return 0;
}

#ifdef _WIN32
unsigned WINAPI Demuxer::Loop(void *arg) {
#else
void* Demuxer::Loop(void *arg){
#endif
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
#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
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
#ifdef _WIN32
    return TerminateThread(&m_thread, 0);
#else
    return pthread_kill(m_thread, SIGQUIT);
#endif
}

void Demuxer::Reset() {
}
