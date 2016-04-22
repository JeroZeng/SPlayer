#include "SPlayer.h"
#include "demuxers/Mp4Demuxer.h"
#include "demuxers/RawYUV.h"
#include "renders/PCRender.h"

SPlayer::SPlayer(const char *url){
    m_chUrl = url;
    //demuxer = new Demuxer();
    //demuxer = new Mp4Demuxer();
    demuxer = new RawYUV();
    decoder = new Decoder();
    //render  = new Render();
    render  = new PCRender(this);
}

SPlayer::~SPlayer(){
    delete demuxer;
    delete decoder;
    delete render;
}

void SPlayer::Init(SWindow *win) {
    demuxer->Open("../flower_352_288.yuv");
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
