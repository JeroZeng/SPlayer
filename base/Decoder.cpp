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

    printf("------------in decoder---------------\n");
    Decoder *decoder = (Decoder*)arg;
    SBucket bucket;
    decoder->m_sQueue->Pop(&bucket);
    while(bucket.size > 0){
        SBucket *rData = new SBucket();
        rData->size = bucket.size << 1;
        rData->data = bucket.data;
        decoder->m_sQueue->Pop(&bucket);
    }
    if (bucket.size == 0) {
        SBucket *rData = new SBucket();
        rData->size = 0;
        decoder->m_sRenderQueue->Push(rData);
    }
    return NULL;
}

int Decoder::WaitStreamEnd(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
