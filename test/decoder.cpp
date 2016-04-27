#include <stdio.h>
#include <stdlib.h>
#include "codec_api.h"

#define BUFFER_2M (2 << 20)
FILE *fp;
bool bReadFile = true;
int32_t iUsedSize = 0;
uint8_t *pBuf;

int ReadFrame(uint8_t **cur) {
    int sliceSize = 0;
    if (bReadFile) {
        printf("------------->Read Data\n");
        fread(pBuf, BUFFER_2M, 1, fp); 
        bReadFile = false;
        iUsedSize = 4;
    }
    uint8_t *pos = pBuf + iUsedSize;
    *cur = pos-3;
    int i = 0;
    for(i=0; i < BUFFER_2M-iUsedSize; i++) {
        if((pos[i] == 0 &&
           pos[i+1] == 0 &&
           pos[i+2] == 0 &&
           pos[i+3] == 1) ||
           (pos[i] == 0 &&
           pos[i+1] == 0 &&
           pos[i+2] == 1)) {
            break;
        }
    }
    printf("------------->find one frame %d\n", i+3);
    iUsedSize += (i+4);
    if (iUsedSize == BUFFER_2M) {
        printf("------------->find one frame\n");
        i = 0;
        iUsedSize = 0;
        bReadFile = true;
    }
    return i+3;
}

int main(int argc, char *argv[]) {
    fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("=======Open File Failed\n");
        return -1;
    }
    ISVCDecoder *pDecoder = NULL; 
    SDecodingParam sDecParam = {0};
    SBufferInfo sDstBufInfo;
    pBuf = (uint8_t*)malloc(BUFFER_2M* sizeof(uint8_t));
    uint8_t *pCurPos = pBuf;
    uint8_t *pDst[3] = {NULL};
    int32_t iSliceSize = 0;

    sDecParam.sVideoProperty.size = sizeof (sDecParam.sVideoProperty);
    sDecParam.uiTargetDqLayer = (uint8_t) - 1;
    sDecParam.eEcActiveIdc = ERROR_CON_SLICE_COPY;
    sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_DEFAULT;

    WelsCreateDecoder (&pDecoder);
    if (NULL == pDecoder) {
        printf("Create Decoder failed.\n");
        return 1;
    }

    if (pDecoder->Initialize (&sDecParam)) {
        printf ("Decoder initialization failed.\n");
        return 1;
    }

    while (true) {
        iSliceSize = ReadFrame(&pCurPos);
        pDecoder->DecodeFrameNoDelay(pCurPos, iSliceSize, pDst, &sDstBufInfo);
        if (sDstBufInfo.iBufferStatus == 1) {
            printf("-------------->\n");
        }
    }

    if (pDecoder) {
        pDecoder->Uninitialize();
        WelsDestroyDecoder(pDecoder);
    }
    free(pBuf); pBuf = NULL;
    fclose(fp);
    return 0;

}
