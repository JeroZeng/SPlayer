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

int Decoder::DecodeOneFrame(SBucket *bucket) {

 return 0;
}

void* Decoder::Loop(void *arg){

    Decoder *decoder = (Decoder*)arg;
    SBucket *bucket = new SBucket();
    decoder->m_sQueue->Pop(&bucket);
    SBucket *rData = new SBucket();
    while(bucket->size > 0){
        rData->size = bucket->size << 1;
        rData->data = bucket->data;
        decoder->m_sRenderQueue->Push(&rData);
        decoder->m_sQueue->Pop(&bucket);
    }
    if (bucket->size == 0) {
        rData->size = 0;
        decoder->m_sRenderQueue->Push(&rData);
    }
    delete rData;
    delete bucket;
    return NULL;
}

int Decoder::WaitStreamEnd(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
