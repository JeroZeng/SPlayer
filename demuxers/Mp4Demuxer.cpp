#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Mp4Demuxer.h"

Mp4Demuxer::Mp4Demuxer(){
    m_iReadAvc1 = 2;
    m_iChunkIndex = 0;
    m_iNextFrameSize = 0;
    m_iNaluTotalSize = 0;
}

Mp4Demuxer::~Mp4Demuxer(){
    if (!m_pFile) {
        fclose(m_pFile);
        m_pFile = NULL;
    }
}

int Mp4Demuxer::Open(const char *url){
    m_pFile = fopen(url, "r");
    if (m_pFile == (FILE*)NULL) {
        return -1;
    }
    printf("------>open mp4\n");
    uint32_t curPos = ftell(m_pFile);
    for (; !feof(m_pFile);) {
        fseek(m_pFile, curPos, SEEK_SET);
        curPos += ReadRootBox();
    }
    return 0;
}

uint32_t Mp4Demuxer::ReadRootBox() {
    int k = 0;
    unsigned char p[5];
    int boxSize = read_uint32_lit();
    if (boxSize == 0) {
        return 1;
    }
    fread(&k, sizeof(k), 1, m_pFile);
    indianA.size = k;
    p[0] = indianA.byte[0];
    p[1] = indianA.byte[1];
    p[2] = indianA.byte[2];
    p[3] = indianA.byte[3];
    p[4] = 0;
    std::string name = (char*)p;
    if (name == "moov") {
        ReadMoovBox(boxSize);
    } else if (name == "ftyp") {
        ReadFtypBox(boxSize);
    } else if (name == "mdat") {
    
    } else if (name == "free") {

    } else {
    }
    return boxSize;
}

void Mp4Demuxer::ReadMoovBox(int size) {
    moov.size = size;
    moov.type = 'm'|'o'<<8|'o'<<16|'v'<<24;
    int k = 0;
    unsigned char p[5];
    int inner_size = 0;
    int boxSize = 0;

    int cur_pos = ftell(m_pFile);
    do {
        fseek(m_pFile, cur_pos, SEEK_SET);
        boxSize = read_uint32_lit();
        fread(&k, sizeof(k), 1, m_pFile);
        indianA.size = k;
        p[0] = indianA.byte[0];
        p[1] = indianA.byte[1];
        p[2] = indianA.byte[2];
        p[3] = indianA.byte[3];
        p[4] = 0;
        std::string name = (char*)p;
        if (name == "mvhd") {
            ReadMvhdBox(boxSize);
        } else if (name == "trak") {
            ReadTrakBox(boxSize);
        } else if (name == "iods") {
            ReadIodsBox(boxSize);
        } else if (name == "udta") {
            ReadUdtaBox(boxSize);
        }
        cur_pos += boxSize;
        inner_size += boxSize;
    } while(inner_size+8 != size);
}

void Mp4Demuxer::ReadFtypBox(int size) {
//TODO
}

void Mp4Demuxer::ReadIodsBox(int size) {
}

void Mp4Demuxer::ReadUdtaBox(int size) {
}

void Mp4Demuxer::ReadMvhdBox(int size) {
    int k = 0;
    unsigned char p[5];
    int inner_size = 0;
    int boxSize = 0;

    moov.mvhd.size = size;
    moov.mvhd.type = ('m'|'v'<<8|'h'<<16|'d'<<24);
    moov.mvhd.version = read_uint8();
    fread(moov.mvhd.flags, sizeof(moov.mvhd.flags), 1, m_pFile);
    moov.mvhd.createTime      = read_uint32_lit();                        //4
    moov.mvhd.modifyTime  = read_uint32_lit();                        //4
    moov.mvhd.timescale          = read_uint32_lit();                        //4
    moov.mvhd.duration           = read_uint32_lit();                        //4
    moov.mvhd.rate               = read_uint32_lit();                        //4
    moov.mvhd.volume             = read_uint16_big();                        //2
    fread(&moov.mvhd.reserved1, sizeof(moov.mvhd.reserved1), 1, m_pFile);         //2
    fread(&moov.mvhd.reserved2, sizeof(moov.mvhd.reserved2), 1, m_pFile);         //8
    fread(moov.mvhd.matrix, sizeof(moov.mvhd.matrix), 1, m_pFile);           //36
    fread(moov.mvhd.preDefined, sizeof(moov.mvhd.preDefined), 1, m_pFile); //24
    moov.mvhd.nextTrackID      = read_uint32_lit();                        //4
}

void Mp4Demuxer::ReadTrakBox(int size) {
    TrakBox *trak = new TrakBox;
    int k = 0;
    unsigned char p[5];
    int inner_size = 0;
    int boxSize = 0;

    int cur_pos = ftell(m_pFile);
    do {
        fseek(m_pFile, cur_pos, SEEK_SET);
        boxSize = read_uint32_lit();
        fread(p, 4, 1, m_pFile);
        p[4] = 0;
        std::string name = (char*)p;
        //printf("%s\n", name.c_str());

        if (name == "tkhd") {
            ReadTkhdBox(trak, boxSize);
        } else if (name == "edts") {
            ReadEdtsBox(trak, boxSize);
        } else if (name == "mdia") {
            ReadMdiaBox(trak, boxSize);
        } else if (name == "udta") {
            ReadUdtaBox(trak, boxSize);
        } else {
            //TODO Unrecognized box
        }
        cur_pos += boxSize;
        inner_size += boxSize;
    } while (inner_size+8 != size);
    moov.trak.push_back(trak);
}

void Mp4Demuxer::ReadUdtaBox(TrakBox *trak, int size) {

}

void Mp4Demuxer::ReadTkhdBox(TrakBox *trak, int size) {
    trak->tkhd.size              = size;
    trak->tkhd.type              = 0;
    trak->tkhd.version           = read_uint8();
    fread(&trak->tkhd.flags, sizeof(trak->tkhd.flags), 1, m_pFile);
    trak->tkhd.createTime     = read_uint32_lit();
    trak->tkhd.modifyTime = read_uint32_lit();
    trak->tkhd.trackID          = read_uint32_lit();
    trak->tkhd.reserved1         = read_uint32_lit();
    trak->tkhd.duration          = read_uint32_lit();
    fread(&trak->tkhd.reserved2, sizeof(trak->tkhd.reserved2), 1, m_pFile);
    trak->tkhd.layer             = read_uint16_big();
    trak->tkhd.alternateGroup   = read_uint16_big();
    trak->tkhd.volume            = read_uint16_big();
    trak->tkhd.reserved3         = read_uint16_big();
    fread(&trak->tkhd.matrix, sizeof(trak->tkhd.matrix), 1, m_pFile);
    trak->tkhd.width             = read_uint32_lit();
    trak->tkhd.height            = read_uint32_lit();
}

void Mp4Demuxer::ReadEdtsBox(TrakBox *trak, int size) {
//TODO
}

void Mp4Demuxer::ReadMdiaBox(TrakBox *trak, int size) {
    trak->mdia.size = size;
    trak->mdia.type = 'm' | 'd'<<8 | 'i'<<16 | 'a'<<24;
    
    unsigned char p[5];
    int innerSize = 0;
    int boxSize = 0;
    int curPos = ftell(m_pFile);
    do {
        fseek(m_pFile, curPos, SEEK_SET);
        boxSize = read_uint32_lit();
        fread(p, 4, 1, m_pFile);
        p[4] = 0;
        std::string name = (char*)p;
        if (name == "mdhd") {
            ReadMdhdBox(trak->mdia.mdhd, boxSize);
        } else if (name == "hdlr") {
            ReadHdlrBox(trak->mdia.hdlr, boxSize);
        } else if (name == "minf") {
            ReadMinfBox(trak->mdia.minf, boxSize);
        } else {
            //TODO Unrecognized box
        }
        curPos += boxSize;
        innerSize += boxSize;
    } while(innerSize+8 != size);
}

void Mp4Demuxer::ReadMdhdBox(MdhdBox &mdhd, int size) {
    mdhd.size              = size;
    mdhd.type              = 0;
    mdhd.version           = read_uint8();
    fread(&mdhd.flags, sizeof(mdhd.flags), 1, m_pFile);
    mdhd.createTime        = read_uint32_lit();
    mdhd.modifyTime        = read_uint32_lit();
    mdhd.timescale         = read_uint32_lit();
    mdhd.duration          = read_uint32_lit();
    mdhd.language          = read_uint16_big();
    mdhd.preDefined        = read_uint16_big();
}

void Mp4Demuxer::ReadHdlrBox(HdlrBox &hdlr, int size) {
    int innerSize = 0;
    hdlr.size            = size;
    hdlr.type            = 'h'|'d'<<8|'l'<<16|'r'<<24;
    hdlr.version         = read_uint8();
    fread(&hdlr.flags, sizeof(hdlr.flags), 1, m_pFile);
    hdlr.preDefined     = read_uint32_lit();
    fread(&hdlr.handlerType, sizeof(hdlr.handlerType), 1, m_pFile);
    hdlr.handlerType[4] = 0;
    fread(&hdlr.reserved, sizeof(hdlr.reserved), 1, m_pFile);
    fread(&innerSize, 1, 1, m_pFile);
    hdlr.name            = new char[innerSize + 1];
    for(int i = 0; i < innerSize; ++i)
        fread(&hdlr.name[i], sizeof(unsigned char), 1, m_pFile);
}

void Mp4Demuxer::ReadMinfBox(MinfBox &minf, int size) {
    unsigned char p[5];
    int innerSize = 0;
    int boxSize = 0;
    int curPos = ftell(m_pFile);

    do {
        fseek(m_pFile, curPos, SEEK_SET);
        boxSize = read_uint32_lit();
        fread(p, 4, 1, m_pFile);
        p[4] = 0;

        std::string name = (char*)p;
        //printf("....%s\n", name.c_str());
        if (name == "vmhd") {
            ReadVmhdBox(minf.vmhd, boxSize);
        } else if (name == "dinf") {
            ReadDinfBox(minf.dinf, boxSize);
        } else if (name == "stbl") {
            ReadStblBox(minf.stbl, boxSize);
        } else if (name == "smhd") {
            ReadSmhdBox(minf.smhd, boxSize);
        } else {
            //TODO Unrecognized box
        }

        curPos += boxSize;
        innerSize += boxSize;
    } while (innerSize+8 != size);
}

void Mp4Demuxer::ReadSmhdBox(SmhdBox &box, int size) {
    box.size             = size;
    box.type             = 's'|'m'<<8|'h'<<16|'d'<<24;
    box.version          = read_uint8();
    fread(&box.flags, sizeof(box.flags), 1, m_pFile);
    box.balance          = read_uint16_big();
    box.reserved         = read_uint16_big();
}

void Mp4Demuxer::ReadVmhdBox(VmhdBox &box, int size) {
    box.size          = size;
    box.type          = 0;
    box.version       = read_uint8();
    fread(&(box.flags), sizeof(box.flags), 1, m_pFile);
    box.graphicsMode = read_uint32_lit();
    fread(&(box.opcolor), sizeof(box.opcolor), 1, m_pFile);
}

void Mp4Demuxer::ReadDinfBox(DinfBox &box, int size) {
//TODO
}

void Mp4Demuxer::ReadStblBox(StblBox &box, int size) {
    unsigned char p[5];
    int innerSize = 0;
    int boxSize = 0;
    int curPos = ftell(m_pFile);
    do {
        fseek(m_pFile, curPos, SEEK_SET);

        boxSize          = read_uint32_lit();
        fread(p, 4, 1, m_pFile);
        p[4]              = 0;
        std::string name = (char*)p;
        //printf("............%s\n", name.c_str());
        if (name == "stsd") {
             ReadStsdBox(box.stsd, boxSize);
        } else if (name == "stts") {
             ReadSttsBox(box.stts, boxSize);
        } else if (name == "stss") {
             ReadStssBox(box.stss, boxSize);
        } else if (name == "stsc") {
             ReadStscBox(box.stsc, boxSize);
        } else if (name == "stsz" || name == "stz2") {
             ReadStszBox(box.stsz, boxSize);
        } else if (name == "stco" || name == "c064") {
             ReadStcoBox(box.stco, boxSize);
        } else if (name == "ctts") {
             //TODO
        } else {
            //Unrecognized box
        }
        curPos += boxSize;
        innerSize += boxSize;
    } while (innerSize+8 != size); 
}

void Mp4Demuxer::ReadStcoBox(StcoBox &box, int size) {
    box.size                         = size;
    box.version                      = read_uint8();
    fread(box.flags, sizeof(box.flags), 1, m_pFile);
    box.chunkOffsetAmount          = read_uint32_lit();
    box.chunksOffset= new uint32_t[box.chunkOffsetAmount];
    for(int i = 0 ; i < box.chunkOffsetAmount; ++i){
        box.chunksOffset[i] = read_uint32_lit();
    }
}

void Mp4Demuxer::ReadStszBox(StszBox &box, int size) {
    box.size                 = size;
    box.version              = read_uint8();
    fread(box.flags, sizeof(box.flags), 1, m_pFile);
    box.samplesSizeIntotal = read_uint32_lit();

    if(box.samplesSizeIntotal == 0){
        box.tableSize = read_uint32_lit();
        box.sampleSizeTable = new uint32_t[box.tableSize];
        for(int i = 0; i < box.tableSize; ++i){
            int k = 0;
            box.sampleSizeTable[i] = read_uint32_lit();
            //printf("sampleSize[%d]: %d\n", i, box.sampleSizeTable[i]);
        }
    }
}

void Mp4Demuxer::ReadStscBox(StscBox &box, int size) {
    box.version    = read_uint8();
    fread(&box.flags, sizeof(box.flags), 1, m_pFile);
    box.mapAmount = read_uint32_lit();

    box.scmap      = new Mp4List[box.mapAmount];
    for(int i = 0; i < box.mapAmount; ++i){
        box.scmap[i].firstChunkNum = read_uint32_lit();
        box.scmap[i].sampleAmountPerTable = read_uint32_lit();
        box.scmap[i].sampleDescriptionID = read_uint32_lit();
    }
}

void Mp4Demuxer::ReadStssBox(StssBox &box, int size) {
    box.size              = size;
    box.version           = read_uint8();
    fread(box.flags, sizeof(box.flags), 1, m_pFile);
    box.entryNum= read_uint32_lit();

    box.syncSampleTable = new uint32_t[box.entryNum];
    for(int i =0; i < box.entryNum; ++i){
        box.syncSampleTable[i] = read_uint32_lit();
    }
}

void Mp4Demuxer::ReadSttsBox(SttsBox &box, int size) {
    box.size                 = size;
    box.version              = read_uint8();
    fread(box.flags, sizeof(box.flags), 1, m_pFile);
    box.entryNum    = read_uint32_lit();

    box.timeToSampleTable = new uint32_t[box.entryNum];
    for(int i = 0; i < box.entryNum; ++i){
        box.timeToSampleTable[i] = read_uint32_lit();
    }
}

void Mp4Demuxer::ReadStsdBox(StsdBox &box, int size) {
    box.size               = size;
    box.version            = read_uint8();
    fread(box.flags, sizeof(box.flags), 1, m_pFile);
    box.entryNum  = read_uint32_lit();

    box.sampleDescription = new SampleDescription[box.entryNum];
    for(int i =0; i < box.entryNum; ++i){
        box.sampleDescription[i].size = read_uint32_lit();
        box.sampleDescription[i].type = read_uint32_lit();

       /*if( box.sampleDescription[i].type
            == (('m'<<24)|('p'<<16)|('4'<<8)|('a'))){
                box.sampleDescription[i].mp4a
                    = ReadMp4aBox(m_pFile, box.sample_description[i].size);
                box.sampleDescription[i].avc1 = 0;

        }else */if(box.sampleDescription[i].type
            == (('a'<<24)|('v'<<16)|('c'<<8)|('1'))){
            ReadAvc1Box(box.sampleDescription[i].avc1, box.sampleDescription[i].size);
                //box.sampleDescription[i].mp4a = 0;
        }/*else if(box.sampleDescription[i].type
            == (('h'<<24)|('v'<<16)|('c'<<8)|('1'))){
                box.sampleDescription[i].hvc1
                    = ReadHvc1Box(m_pFile, box.sampleDescription[i].size);
                box.sampleDescription[i].mp4a = 0;
        }*/
    }
}

void Mp4Demuxer::ReadAvc1Box(Avc1Box &box, int size) {
    box.size                   = size;
    box.type                   = (('a'<<24)|('v'<<16)|('c'<<8)|('1'));

    fread(box.reserved, sizeof(box.reserved), 1, m_pFile);  //6
    box.dataRefIndex           = read_uint16_lit();   //2
    box.preDefined             = read_uint16_big();   //2
    box.reserved1              = read_uint16_big();   //2
    fread(box.preDefined1, sizeof(box.preDefined1), 1, m_pFile);  //3*4
    box.width                  = read_uint16_lit();   //2
    box.height                 = read_uint16_lit();   //2
    box.horizRes              = read_uint32_lit();   //4
    box.vertRes               = read_uint32_lit();   //4
    box.reserved2              = read_uint32_lit();   //4
    box.framesCount            = read_uint16_lit();   //2
    fread(box.compressorName, sizeof(box.compressorName), 1, m_pFile);  //33
    box.bitDepth               = read_uint8();        //1
    box.preDefined2            = read_uint16_big();   //2

    //avcC --AVCDecoderConfigurationRecord
    ReadAvcCBox(box.avcC);
}

void Mp4Demuxer::ReadAvcCBox(AvcCBox &avcC) {
    avcC.size                      = read_uint32_lit();

    avcC.type                      = read_uint32_lit();

    avcC.configurationVersion      = read_uint8();
    avcC.AVCProfileIndication      = read_uint8();
    avcC.profileCompatibility      = read_uint8();
    avcC.AVCLevelIndication        = read_uint8();
    avcC.lengthSizeMinusOne        = 0x3 & read_uint8();  //2 bit

    avcC.numOfSequenceParameterSet = 0x1F & read_uint8();  //5bit
    avcC.sps = new AvcCBox::SPS[avcC.numOfSequenceParameterSet];
    for( int i = 0; i < avcC.numOfSequenceParameterSet; ++i ){
        avcC.sps[i].sequenceParameterSetLength = read_uint16_lit();

        avcC.sps[i].sequenceParameterSetNALUnit =
            new uint8_t[avcC.sps[i].sequenceParameterSetLength];
        fread((avcC.sps[i].sequenceParameterSetNALUnit),
              (avcC.sps[i].sequenceParameterSetLength), 1, m_pFile);

        for(int j = 0; j < avcC.sps[i].sequenceParameterSetLength; ++j){
        }
    }

    avcC.numOfPictureParameterSets = read_uint8();
    avcC.pps = new AvcCBox::PPS[avcC.numOfPictureParameterSets];
    for (int i = 0; i < avcC.numOfPictureParameterSets; ++i){
        avcC.pps[i].pictureParameterSetLength = read_uint16_lit();

        avcC.pps[i].pictureParameterSetNALUnit
            = new uint8_t[avcC.pps[i].pictureParameterSetLength];

        fread(avcC.pps[i].pictureParameterSetNALUnit,
              avcC.pps[i].pictureParameterSetLength, 1, m_pFile);

        for (int j = 0; j < avcC.pps[i].pictureParameterSetLength; ++j){
        }
    }
}

int Mp4Demuxer::GetSampleNumCurChunk(const StscBox &box) {
    int sampleNum = 0;

    for (int i=0; i<box.mapAmount; i++) {
        if (i+1 == box.mapAmount) {
            sampleNum = box.scmap[i].sampleAmountPerTable;
        }

        if (m_iChunkIndex+1 >= box.scmap[i].firstChunkNum &&
            m_iChunkIndex+1 < box.scmap[i+1].firstChunkNum) {
            sampleNum = box.scmap[i].sampleAmountPerTable;
            break;
        }
    }

    return sampleNum;
}

int Mp4Demuxer::GetSampleIndex(const StscBox &box, int ci) {
    int sampleIndex = 0;

    for (int i=0; i<box.mapAmount; i++) {
        int chunkAmount = 0;
        int curChunkNum = box.scmap[i].firstChunkNum;

        if (i+1 == box.mapAmount) {
            chunkAmount = ci - curChunkNum;
            sampleIndex += box.scmap[i].sampleAmountPerTable * chunkAmount;
            break;
        }
        int nextChunkNum = box.scmap[i+1].firstChunkNum;
        if (ci > curChunkNum) {
            if (ci < nextChunkNum) {
                chunkAmount = ci - curChunkNum;
                sampleIndex += box.scmap[i].sampleAmountPerTable*chunkAmount;
                break;
            } else {
                chunkAmount = nextChunkNum - curChunkNum;
                sampleIndex += box.scmap[i].sampleAmountPerTable*chunkAmount;
            }
        } else if (ci == curChunkNum) {
            break;
        }
    }
    return sampleIndex;
}

int Mp4Demuxer::GetSampleSize(const StszBox &box, int sampleIndex) {
    for (int i=0; i<box.tableSize; i++) {
        if (sampleIndex == i) {
            return box.sampleSizeTable[i];
        }
    }
    return 0;
}

void Mp4Demuxer::GetNextSample(const StblBox &box) {
    if (m_iSampleNum == GetSampleNumCurChunk(box.stsc)) {
        m_iChunkIndex++;
        m_uiVReaderPos = box.stco.chunksOffset[m_iChunkIndex];
        m_iSampleNum = 0;
    }
    int sampleIndex = GetSampleIndex(box.stsc, m_iChunkIndex+1);

    int sampleSize = GetSampleSize(box.stsz, sampleIndex + m_iSampleNum);
    fseek(m_pFile, m_uiVReaderPos, SEEK_SET);
    m_uiVReaderPos += 4;
    m_iNextFrameSize = read_uint32_lit();
    m_iNaluTotalSize += (m_iNextFrameSize+4);
    if (m_iNaluTotalSize >= sampleSize) {
        m_iSampleNum++;
        m_iNaluTotalSize = 0;
    }
}

int Mp4Demuxer::GetOneFrame(SBucket *bucket){
    bucket->data[0] = 0x00;
    bucket->data[1] = 0x00;
    bucket->data[2] = 0x01;
    if (m_iReadAvc1 > 0) {
        m_iReadAvc1--;
        AvcCBox *avcC = &(moov.trak[0]->mdia.minf.stbl.stsd.sampleDescription[0].avc1.avcC);
        if (m_iReadAvc1 == 1) {
            memcpy((bucket->data+3), avcC->sps[0].sequenceParameterSetNALUnit, avcC->sps[0].sequenceParameterSetLength);
            bucket->size = avcC->sps[0].sequenceParameterSetLength + 3;
            return avcC->pps[0].pictureParameterSetLength + 3;
        } else if (m_iReadAvc1 == 0) {
            memcpy((bucket->data+3), avcC->pps[0].pictureParameterSetNALUnit, avcC->pps[0].pictureParameterSetLength);
            bucket->size = avcC->pps[0].pictureParameterSetLength + 3;
            m_uiVReaderPos = moov.trak[0]->mdia.minf.stbl.stco.chunksOffset[0];
            GetNextSample(moov.trak[0]->mdia.minf.stbl);
            return m_iNextFrameSize + 3;
        }
    }    

    if (m_iChunkIndex < moov.trak[0]->mdia.minf.stbl.stco.chunkOffsetAmount) {
        m_uiVReaderPos += m_iNextFrameSize;
        fread(bucket->data+3, sizeof(char), m_iNextFrameSize, m_pFile);
        //printf("%d\t%d\t%d\t%d\t", m_iNextFrameSize+3, bucket->data[4], bucket->data[5], bucket->data[6]);
        bucket->size = m_iNextFrameSize + 3;
        GetNextSample(moov.trak[0]->mdia.minf.stbl);
    } else {
        bucket->size = 0;
        return 0;
    }

    return m_iNextFrameSize + 3;
}
