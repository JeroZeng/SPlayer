#include "Decoder.h"

Decoder::Decoder(){

}

Decoder::~Decoder(){

    delete m_sQueue;

}

void Decoder::Init(){

}

int Decoder::Start(SQueue *queue){

    m_sQueue = queue;
    m_sRenderQueue = new RenderQueue(5);
    pthread_create(&m_pThreadDecoder, NULL, Decoder::Loop, (void*)this);

    return 0;
}

int Decoder::Recieve(char *dt){

    int size = 0;

    return size;
}

int Decoder::DecodeOneFrame(char *frame, int size) {

 return 0;
}

void* Decoder::Loop(void *arg){

    printf("------------in decoder---------------\n");
    Decoder *decoder = (Decoder*)arg;
    SBucket *bucket = decoder->m_sQueue->Pop();
    while(bucket->size > 0){
        SBucket *rData = new SBucket();
        rData->size = bucket->size << 1;
        rData->data = bucket->data;
        decoder->m_sRenderQueue->Push(rData);
        delete bucket;
        bucket = decoder->m_sQueue->Pop();
    }
    if (bucket->size == 0) {
        SBucket *rData = new SBucket();
        rData->size = 0;
        decoder->m_sRenderQueue->Push(rData);
        delete bucket;
    }
    return NULL;
}

int Decoder::WaitStreamEnd(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
