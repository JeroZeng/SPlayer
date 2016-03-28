#include "Decoder.h"

Decoder::Decoder(){

}

Decoder::~Decoder(){

}

void Decoder::Init(){

}

int Decoder::Start(SQueue *queue){

    m_sQueue = queue;
    printf("------------in decoder---------------\n");
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
    while(1){
        SData *data = decoder->m_sQueue->Pop();    
        printf("%d\n<--------", data->num);
    }
    
    return NULL;
}

int Decoder::WaitThread(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
