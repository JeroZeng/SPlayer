#ifndef _MPEGTS_H_
#define _MPEGTS_H_
#include "../base/Demuxer.h"

class TSDemuxer : public Demuxer {

public:
    TSDemuxer();
    ~TSDemuxer();
    int Open(const char *url);
    int GetOneFrame(SBucket *bucket);
private:
    int DemuxPacket(SBucket *bucket);

public:
    unsigned char m_buf[192];
    int m_iBufLen;

private:
    FILE *m_pFile;

};

#endif
