#ifndef _SPLAYER_H_
#define _SPLAYER_H_
#include "base/Demuxer.h"
#include "base/Decoder.h"
#include "base/Render.h"

class SPlayer{
public:
    Demuxer *demuxer;
    Decoder *decoder;
    Render  *render;

public:
    SPlayer(const char* name);
    ~SPlayer();
    void Init();
    int Play();
    int Pause();
    int Resume();
    int Stop();

public:
    const char* m_chUrl;
};
#endif//_SPLAYER_H_
