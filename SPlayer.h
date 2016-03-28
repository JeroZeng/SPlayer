#include "base/Demuxer.h"
#include "base/Decoder.h"

class SPlayer{
public:
    Demuxer *demuxer;
    Decoder *decoder;

public:
    SPlayer(const char* name);
    ~SPlayer();
    void Init();
    int Play();
};
