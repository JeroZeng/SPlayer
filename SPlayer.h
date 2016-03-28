#include "base/Demuxer.h"

class SPlayer{
public:
    Demuxer *demuxer;

public:
    SPlayer(const char* name);
    ~SPlayer();
    int play();
};
