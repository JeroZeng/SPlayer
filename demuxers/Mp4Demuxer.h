#ifndef _MP4_DEMUXER_
#define _MP4_DEMUXER_

#include "../base/Demuxer.h"
#include <vector>

static union ____indian_swap{
    unsigned char byte[4];
    unsigned int size;
}indianA , indianB;

class BaseBox {
public:
    uint32_t type;
    uint32_t size;
};

class FullBox : public BaseBox {
public:
    uint32_t version;
    uint8_t flags[3];
};

class AvcCBox: public BaseBox {
public:
    uint8_t       configurationVersion;      //=1
    uint8_t       AVCProfileIndication;
    uint8_t       profileCompatibility;
    uint8_t       AVCLevelIndication;

    uint8_t       lengthSizeMinusOne;        // & 0x3,  ==2 bit
    uint8_t       numOfSequenceParameterSet; // & 0x1F  ==5bit
    struct SPS{
        uint16_t  sequenceParameterSetLength;
        uint8_t   *sequenceParameterSetNALUnit;
    }             *sps;

    uint8_t       numOfPictureParameterSets;
    struct PPS{
        uint16_t  pictureParameterSetLength;
        uint8_t   *pictureParameterSetNALUnit;
    }             *pps;
};

class Avc1Box: public BaseBox {
public:
    char          reserved[6];
    uint16_t      dataRefIndex;
    uint16_t      preDefined;
    uint16_t      reserved1;
    uint32_t      preDefined1[3];
    uint16_t      width;
    uint16_t      height;
    uint32_t      horizRes;
    uint32_t      vertRes;
    uint32_t      reserved2;
    /**/uint16_t  framesCount;
    /*AVCDecoderConfigurationRecord*/
    /**/char      compressorName[33];
    /**/uint16_t  bitDepth;
    /**/int16_t   preDefined2;
    AvcCBox avcC;
    struct BtrtBox *btrt;
    //ext descriptors
    struct M4dsBox *m4ds;
public:
    ~Avc1Box() {
        if (btrt != NULL) {
            //delete btrt; btrt = NULL;
        }
        if (m4ds != NULL) {
            //delete m4ds; m4ds = NULL;
        }
    }
};

class TkhdBox: public FullBox {
public:
    uint32_t createTime;
    uint32_t modifyTime;
    uint32_t trackID;
    uint32_t reserved1;
    uint32_t duration;

    uint32_t reserved2[2];
    uint16_t layer;
    uint16_t alternateGroup;

    uint16_t volume;
    uint16_t reserved3;
    uint32_t matrix[9];
    uint32_t width;
    uint32_t height;
};

class EditList{
public:
    uint32_t trakDuration;
    uint32_t duration;
    uint32_t rate;
};

class ElstBox: public FullBox {
public:
    uint32_t amount;
    EditList *editList;
public:
    ~ElstBox() {
        if (editList != NULL) {
            delete editList; editList = NULL;
        }
    }
};

class MdhdBox: public FullBox {
public:
    uint32_t createTime;
    uint32_t modifyTime;
    uint32_t timescale;
    uint32_t duration;
    uint16_t language;
    uint16_t preDefined;
};

class VmhdBox: public FullBox{
public:
    uint32_t graphicsMode;
    uint8_t opcolor[4];  //==============take care
};

class DrefBox: public FullBox {
public:
    uint32_t entryNum;
    //there are many urls
    //======================================
};

struct Mp4List{
    uint32_t firstChunkNum;
    uint32_t sampleAmountPerTable;
    uint32_t sampleDescriptionID;
};

class StscBox: public FullBox {  //Sample-to-Chunk Box
public:
    uint32_t mapAmount;
    Mp4List *scmap; //sample-to-trunk
public:
    ~StscBox() {
        if (scmap != NULL) {
            delete scmap;
        }
    }
};

class StszBox: public FullBox {  //Samples Size
public:
    uint32_t samplesSizeIntotal;
    uint32_t tableSize;
    uint32_t *sampleSizeTable;
public:
    ~StszBox() {
        if (sampleSizeTable != NULL) {
            free(sampleSizeTable); sampleSizeTable = NULL;
        }
    }
};

class StcoBox: public FullBox {  //Chunk Offset
public:
    uint32_t chunkOffsetAmount;
    uint32_t *chunksOffset;//From file beginning 
public:
    ~StcoBox() {
        if (chunksOffset != NULL) {
            free(chunksOffset); chunksOffset = NULL;
        }
    }
};

class StssBox: public FullBox {  //Sync Sample
public:
    uint32_t entryNum;
    uint32_t *syncSampleTable;
public:
     ~StssBox() {
        if (syncSampleTable != NULL) {
            free(syncSampleTable);
            syncSampleTable = NULL;
        }
    }
};

class SttsBox: public FullBox {  //Time-to-Sample
public:
    uint32_t entryNum;
    uint32_t *timeToSampleTable;
public:
    ~SttsBox() {
        if (timeToSampleTable != NULL) {
            free(timeToSampleTable);
            timeToSampleTable = NULL;
        }
    }
};

struct SampleDescription{
    uint32_t            size;
    uint32_t            type;
    Avc1Box avc1;
    //Mp4aBox *mp4a;
    //Hvc1Box *hvc1;
};

class StsdBox: public FullBox {
public:
    uint32_t            entryNum;  //4 bytes
    SampleDescription *sampleDescription;
public:
    ~StsdBox() {
        if (sampleDescription != NULL) {
            delete sampleDescription;
            sampleDescription = NULL;
        }
    }
};

class SmhdBox: public FullBox {  //sound media header box
public:
    uint16_t            balance;
    uint16_t            reserved;
};

class HdlrBox: public FullBox {
public:
    uint32_t       preDefined;
    uint8_t        handlerType[5];
    uint32_t       reserved[3];
    char           *name;  //end with '\0'
public:
    ~HdlrBox() {
        if (name != NULL) {
            free(name); name = NULL;
        }
    }
};

class MdatBox: public BaseBox {
public:
    uint64_t dataSize;
    char     *data;
public:
    ~MdatBox() {
        if (data != NULL) {
            free(data); data = NULL;
        }
    }
};

class MvhdBox: public FullBox {
public:
    uint32_t createTime;      //4
    uint32_t modifyTime;  //4
    uint32_t timescale;          //4
    uint32_t duration;           //4
    uint32_t rate;               //4
    uint16_t volume;             //2
    uint16_t reserved1;          //2
    uint32_t reserved2[2];       //8
    uint32_t matrix[9];          //36
    uint32_t preDefined[6];     //24
    uint32_t nextTrackID;      //4
};

class StblBox{
public:
    StsdBox stsd;
    SttsBox stts;
    StssBox stss;
    StscBox stsc;
    StszBox stsz;
    StcoBox stco;
};

class DinfBox {};
class EdtsBox {};

class MinfBox: public BaseBox {
public:
    VmhdBox vmhd;
    SmhdBox smhd;
    HdlrBox hdlr;
    DinfBox dinf;
    StblBox stbl;
};

class MdiaBox: public BaseBox {
public:
    MdhdBox mdhd;
    HdlrBox hdlr;
    MinfBox minf;
};

class UdtaBox {};
class TrakBox{
public:
    TkhdBox tkhd;
    MdiaBox mdia;
    EdtsBox edts;
    UdtaBox udta;
};

class IodsBox {};
class MoovBox: public BaseBox {
public:
    MvhdBox mvhd;
    IodsBox iods;
    std::vector<TrakBox *> trak;
    UdtaBox udta;
public:
    ~MoovBox() {
        for(int i=0; i<trak.size(); i++) {
            if (trak.at(i) != NULL) {
                delete trak[i];
                trak[i] = NULL;
            }
        }
    }
};

class Mp4Demuxer: public Demuxer {
public:
    Mp4Demuxer();
    ~Mp4Demuxer();
    int Open(const char *url);
    int GetOneFrame(SBucket *data);
    void GetNextSample(const StblBox &box);
    int GetSampleSize(const StszBox &box, int sampleIndex);
    int GetSampleIndex(const StscBox &box, int chunkIndex);
    int GetSampleNumCurChunk(const StscBox  &box);

public:
    int m_iReadAvc1;
    int m_iChunkIndex;
    int m_iSampleNum;
    uint32_t m_uiVReaderPos;
    int m_iNextFrameSize;
    int m_iNaluTotalSize;
    MoovBox moov;

public:
    unsigned read_uint16_big()
    {
        unsigned short k = 0;
        fread(&k, sizeof(k), 1, m_pFile);
        return k;
    }
    uint16_t read_uint16_lit()
    {
        uint16_t k;
        fread(&k, sizeof(k), 1, m_pFile);
        return ((k&0xff00) >> 8)|((k&0xff) << 8);
    }

    unsigned char read_uint8()
    {
        unsigned char x;
        fread(&x, sizeof(x), 1, m_pFile);
        return x;
    }

    int read_uint32_lit(){
        int k = 0;
        fread(&k, sizeof(k), 1, m_pFile);
        indianA.size = k;
        for(int i = 0, j = 3; i < 4; i++, j--)
            indianB.byte[i] = indianA.byte[j];

        return indianB.size;
    }
    uint32_t ReadRootBox();
    void ReadMoovBox(int size);
    void ReadFtypBox(int size);
    void ReadMvhdBox(int size);
    void ReadTrakBox(int size);
    void ReadIodsBox(int size);
    void ReadUdtaBox(int size);
    void ReadTkhdBox(TrakBox *trak, int size);
    void ReadEdtsBox(TrakBox *trak, int size);
    void ReadMdiaBox(TrakBox *trak, int size);
    void ReadUdtaBox(TrakBox *trak, int size);
    void ReadMdhdBox(MdhdBox &mdhd, int size);
    void ReadHdlrBox(HdlrBox &hdlr, int size);
    void ReadMinfBox(MinfBox &minf, int size);
    void ReadSmhdBox(SmhdBox &smhd, int size);
    void ReadVmhdBox(VmhdBox &vmhd, int size);
    void ReadDinfBox(DinfBox &dinf, int size);
    void ReadStblBox(StblBox &stbl, int size);
    void ReadStsdBox(StsdBox &stsd, int size);
    void ReadAvc1Box(Avc1Box &avc1, int size);
    void ReadAvcCBox(AvcCBox &avcC);
    void ReadSttsBox(SttsBox &stts, int size);
    void ReadStssBox(StssBox &stss, int size);
    void ReadStscBox(StscBox &stsc, int size);
    void ReadStszBox(StszBox &stsz, int size);
    void ReadStcoBox(StcoBox &stco, int size);
private:
    FILE *m_pFile;
};

#endif//_MP4_DEMUXER_
