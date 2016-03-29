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
    m_sQueueRender = new SQueue();
    pthread_create(&m_pThreadDecoder, NULL, Decoder::Loop, (void*)this);

    return 0;
}

int Decoder::Recieve(char *dt){

    int size = 0;

    return size;
}

void* Decoder::Loop(void *arg){

    printf("------------in decoder---------------\n");
    Decoder *decoder = (Decoder*)arg;
    SData *data = decoder->m_sQueue->Pop();
    while(data->num > 0){
        printf("%d<--------\n", data->num);
        delete data;
        data = decoder->m_sQueue->Pop();
    }
    if (data->num == 0) {
        delete data;
    }
    return NULL;
}

int Decoder::WaitThread(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
