#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"
#include "demuxers/RawYUV.h"
#include "demuxers/H264Demuxer.h"
#include "decoders/H264Decoder.h"
#include "renders/PCRender.h"

enum FileType {
    Type_YUV = 'y' + 'u' + 'v',
    Type_264 = '2' + '6' + '4',
};

SPlayer::SPlayer(const char *url){
    int urlSize = strlen(url);
    int fileType = 0;
    int i = urlSize;
    m_chUrl = url;
    while(i>0) {
        i--;
        if (m_chUrl[i] == '.') {
            while (i<urlSize) {
                i++;
                fileType += m_chUrl[i];
            }
            break;
        }
    }
    switch(fileType) {
        case Type_YUV: {
            demuxer = new RawYUV();
            decoder = new Decoder();
        } break;
        case Type_264: {
            demuxer = new H264Demuxer();
            decoder = new H264Decoder(this);
        } break;
    }
    render  = new Render();
    render  = new PCRender(this);
}

SPlayer::~SPlayer(){
    delete demuxer;
    delete decoder;
    delete render;
}

void SPlayer::Init(SWindow *win) {
    demuxer->Open(m_chUrl);
    decoder->Init();
    render->m_iWidth = decoder->m_iWidth = demuxer->m_iWidth;
    render->m_iHeight = decoder->m_iHeight = demuxer->m_iHeight;
    render->Init(win);
    render->SetFPS(demuxer->m_dFPS);
}

int SPlayer::Play() {
    printf("---------->Start Play\n");
    demuxer->Start();
    decoder->Start(demuxer->m_sQueue);
    render->Start(decoder->m_sRenderQueue);
    return 0;
}

int SPlayer::Pause() {
    return render->Pause();
}

int SPlayer::Resume() {
    return render->Resume();
}

int SPlayer::Stop() {
    //demuxer->Stop();
    //decoder->Stop();
    render->Stop();
    //render->WaitStreamEnd();
    decoder->WaitThreadClose();
    demuxer->WaitThreadClose();
    decoder->ClearMem();
    demuxer->ClearMem();
    return 0;
}
