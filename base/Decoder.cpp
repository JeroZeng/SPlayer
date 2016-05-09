#include "Decoder.h"

Decoder::Decoder(){
}

Decoder::~Decoder(){
}

int Decoder::Init(){
    return  0;
}

int Decoder::Start(SQueue *queue){

    m_sQueue = queue;
    m_sRenderQueue = new RenderQueue(RQ_SIZE);
#ifdef _WIN32
    m_thread = (HANDLE)_beginthreadex(NULL, 0, (Decoder::Loop), (void*)this, 0, NULL);
#else
    pthread_create(&m_thread, NULL, Decoder::Loop, (void*)this);
#endif
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

#ifdef _WIN32
unsigned WINAPI Decoder::Loop(void *arg) {
#else
void* Decoder::Loop(void *arg){
#endif
    Decoder *decoder = (Decoder*)arg;
    SBucket *db = new SBucket();
    decoder->m_sQueue->Pop(&db);
    SBucket *rb = new SBucket();
#ifdef _DEBUG_
    int frame_num = 0;
#endif
    if (decoder->m_iWidth*decoder->m_iHeight>0)
        decoder->m_MemBar[0] = (unsigned char*)MALLOC(decoder->m_iWidth*
                    decoder->m_iHeight*sizeof(unsigned char) * 3 / 2);
    rb->data = decoder->m_MemBar[0];
    for (int i=0; (i<RQ_SIZE+2)&&(db->size>0); ) {
        if (decoder->DecodeOneFrame(db, rb) == RES_OK) {
            decoder->m_sRenderQueue->Push(&rb);
            i++;
#ifdef _DEBUG_
            frame_num++;
#endif
        }
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
#ifdef _DEBUG_
            frame_num++;
#endif
        }
        decoder->m_sQueue->Pop(&db);
    }
    decoder->m_sQueue->StopReader();
    if (db->size == 0) {
        rb->size = 0;
        decoder->m_sRenderQueue->Push(&rb);
    }
#ifdef _DEBUG_
    printf("---------->Decode %d Frames.\n", frame_num);
#endif
    delete rb; rb = NULL;
    delete db; db = NULL;
#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

int Decoder::Stop() {
#ifdef _WIN32
    return TerminateThread(m_thread, 0);
#else
    return pthread_kill(m_thread, SIGQUIT);
#endif
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
