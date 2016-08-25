#include "Mpegts.h"

TSDemuxer::TSDemuxer() {
    m_iBufLen = 0;
}

TSDemuxer::~TSDemuxer() {
    if (m_pFile != (FILE*)NULL)
        fclose(m_pFile);
}

int TSDemuxer::Open(const char *url) {
    m_pFile = fopen(url, "rb");
    if (m_pFile == (FILE*)NULL) {
         return -1;
    }
    printf("------>open ts\n");
    return 0;
}

int TSDemuxer::DemuxPacket(SBucket *bucket) {
    
    return 0;
}

int TSDemuxer::GetOneFrame(SBucket *bucket) {
    while(true) {
        if (m_iBufLen) {
            fread(m_buf, sizeof(char), m_iBufLen, m_pFile);
        } else {
            fread(m_buf, sizeof(char), 188, m_pFile);
            if (m_buf[0] == 0x47 && m_buf[4] != 0x47) {
                m_iBufLen = 188;
                printf("This is TS Stream.\n");
            } else if (m_buf[0] != 0x47 && m_buf[4] == 0x47) {
                m_iBufLen= 192;
                fread(m_buf+188, sizeof(char), 4, m_pFile);
                printf("This is M2TS Stream.\n");
            } else {
                printf("Err: Unknow Stream Type\n");
                return -1;
            }
        }

        if(DemuxPacket(bucket)) {
            return -1;
        }

    }
    return 0;
}
