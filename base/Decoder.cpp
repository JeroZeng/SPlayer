#include "Decoder.h"

Decoder::Decoder(){
}

Decoder::~Decoder(){
}

void Decoder::Init(){

}

int Decoder::Start(SQueue *queue){

    m_sQueue = queue;
    m_sRenderQueue = new RenderQueue(RQ_SIZE);
    pthread_create(&m_thread, NULL, Decoder::Loop, (void*)this);

    return 0;
}

int Decoder::Recieve(char *dt){

    int size = 0;

    return size;
}

int Decoder::DecodeOneFrame(SBucket *db, SBucket *rb) {
    rb->size = m_iWidth * m_iHeight * 3 / 2;
    memcpy(rb->data, db->data, db->size);
    return 0;
}

void* Decoder::Loop(void *arg){
    Decoder *decoder = (Decoder*)arg;
    SBucket *db = new SBucket();
    decoder->m_sQueue->Pop(&db);
    SBucket *rb = new SBucket();
    if (decoder->m_iWidth*decoder->m_iHeight>0)
        decoder->m_MemBar[0] = (unsigned char*)MALLOC(decoder->m_iWidth*
                    decoder->m_iHeight*sizeof(unsigned char) * 3 / 2);
    rb->data = decoder->m_MemBar[0];
    for (int i=1; (i<RQ_SIZE+2)&&(db->size>0); i++) {
        if (decoder->DecodeOneFrame(db, rb) == RES_OK)
            decoder->m_sRenderQueue->Push(&rb);
        if (decoder->m_iWidth*decoder->m_iHeight>0)
            decoder->m_MemBar[i] = (unsigned char*)MALLOC(decoder->m_iWidth*
                    decoder->m_iHeight*sizeof(unsigned char) * 3 / 2);
        rb->data = decoder->m_MemBar[i];
        decoder->m_sQueue->Pop(&db);
    }
    while(db->size > 0){
        if (decoder->DecodeOneFrame(db, rb) == RES_OK) {
            if(decoder->m_sRenderQueue->Push(&rb)) {
                break;
            }
        }
        decoder->m_sQueue->Pop(&db);
    }
    decoder->m_sQueue->StopReader();
    if (db->size == 0) {
        rb->size = 0;
        decoder->m_sRenderQueue->Push(&rb);
    }
    delete rb; rb = NULL;
    delete db; db = NULL;
    return NULL;
}

int Decoder::Stop() {
    return pthread_kill(m_thread, SIGQUIT);
}

void Decoder::ClearMem() {
    if (m_sRenderQueue == NULL)
        return;
    delete m_sRenderQueue;
    m_sRenderQueue = NULL;
    // free memory
    for (int i=0; i<RQ_SIZE+2; i++) {
        if (m_MemBar[i] != NULL) {
            free(m_MemBar[i]);
            m_MemBar[i] = NULL;
        }
    }
}
