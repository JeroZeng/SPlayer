#include "Decoder.h"

#define RQ_SIZE 3 

Decoder::Decoder(){

}

Decoder::~Decoder(){

    delete m_sQueue;

}

void Decoder::Init(){

}

int Decoder::Start(SQueue *queue){

    m_sQueue = queue;
    m_sRenderQueue = new RenderQueue(RQ_SIZE);
    pthread_create(&m_pThreadDecoder, NULL, Decoder::Loop, (void*)this);

    return 0;
}

int Decoder::Recieve(char *dt){

    int size = 0;

    return size;
}

int Decoder::DecodeOneFrame(SBucket *db, SBucket *rb) {
    rb->size = m_iWidth * m_iHeight;
    memcpy(rb->data, db->data, db->size);
    return 0;
}

void* Decoder::Loop(void *arg){
    char *memBar[RQ_SIZE+2];
    Decoder *decoder = (Decoder*)arg;
    SBucket *db = new SBucket();
    decoder->m_sQueue->Pop(&db);
    SBucket *rb = new SBucket();
    memBar[0] = (char*)MALLOC(decoder->m_iWidth*decoder->m_iHeight*
                    sizeof(char));
    rb->data = memBar[0];
    for (int i=1; (i<RQ_SIZE+2)&&(db->size>0); i++) {
        decoder->DecodeOneFrame(db, rb);
        decoder->m_sRenderQueue->Push(&rb);
        memBar[i] = (char*)MALLOC(decoder->m_iWidth*decoder->m_iHeight*
                        sizeof(char));
        rb->data = memBar[i];
        decoder->m_sQueue->Pop(&db);
    }
    while(db->size > 0){
        decoder->DecodeOneFrame(db, rb);
        decoder->m_sRenderQueue->Push(&rb);
        decoder->m_sQueue->Pop(&db);
    }
    if (db->size == 0) {
        rb->size = 0;
        decoder->m_sRenderQueue->Push(&rb);
    }
    for (int i=0; i<RQ_SIZE+1; i++) {
        if (memBar[i] != NULL) {
            //TODO free(memBar[i]);
        }
        memBar[i] = NULL;
    }
    delete rb;
    delete db;
    return NULL;
}

int Decoder::WaitStreamEnd(){

    pthread_join(m_pThreadDecoder, NULL);
    return 0;
}
