#ifndef _MP4_DEMUXER_
#define _MP4_DEMUXER_

#include "../base/Demuxer.h"

class Mp4Demuxer: public Demuxer {
public:
    Mp4Demuxer();
    ~Mp4Demuxer();
    int Open(const char *url);
    int GetOneFrame(char *data);

private:
    FILE *fp;
};

#endif//_MP4_DEMUXER_
