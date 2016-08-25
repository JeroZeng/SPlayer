#include "Mpegts.h"

Mpegts::Mpegts() {
}

Mpepts::~Mpegts() {
    if (m_pFile != (FILE*)NULL)
        fclose(m_pFile);
}

int Mpegts::Open(const char *url) {
    m_pFile = fopen(url, "rb");
    if (m_pFile == (FILE*)NULL) {
         return -1;
    }
    printf("------>open ts\n");
    return 0;
}

int GetOneFrame(SBucket *bucket) {
    return 0;
}
