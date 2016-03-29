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
    SData *data = decoder->m_sQueue->Pop();
    while(data->size > 0){
        SData *rData = new SData();
        rData->size = data->size << 1;
        rData->data = data->data;
        decoder->m_sRenderQueue->Push(rData);
        delete data;
        data = decoder->m_sQueue->Pop();
    }
    if (data->size == 0) {
        SData *rData = new SData();
        rData->size = 0;
        decoder->m_sRenderQueue->Push(rData);
        delete data;
    }
    return NULL;
}

int Decoder::WaitStreamEnd(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
