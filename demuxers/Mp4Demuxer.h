#ifndef _MP4_DEMUXER_
#define _MP4_DEMUXER_

#include "../base/Demuxer.h"

class Mp4Demuxer: public Demuxer
{
public:
    int Open();
    int GetOneFrame(char *data);
};

#endif//_MP4_DEMUXER_
