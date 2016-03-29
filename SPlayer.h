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
};
