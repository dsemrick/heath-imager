#ifndef __H17DISK_H__
#define __H17DISK_H__

#include "sector.h"
#include "raw_sector.h"
#include "raw_track.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>


class H17Disk
{
public:
    H17Disk();
    ~H17Disk();

    static const uint8_t DiskFormatBlock_c = 0x00;
    static const uint8_t FlagsBlock_c      = 0x01;
    static const uint8_t LabelBlock_c      = 0x02;
    static const uint8_t CommentBlock_c    = 0x03;
    static const uint8_t DateBlock_c       = 0x04;
    static const uint8_t ImagerBlock_c     = 0x05;
    static const uint8_t ProgramBlock_c    = 0x06;
   
    static const uint8_t DataBlock_c       = 0x10;
    static const uint8_t RawDataBlock_c    = 0x30;

    static const uint8_t TrackDataId       = 0x11;
    static const uint8_t SectorDataId      = 0x12;

    static const uint8_t RawTrackDataId    = 0x31;
    static const uint8_t RawSectorDataId   = 0x32;

    virtual bool openForWrite(char *name);
    virtual bool openForRead(char *name);
    virtual bool openForRecovery(char *name);

    virtual bool decodeFile(char *name);

    virtual bool decodeBuffer(unsigned char buf[], unsigned int size);
    virtual bool validateHeader(unsigned char buf[], unsigned int size, unsigned int &length);
    virtual bool validateBlock(unsigned char buf[], unsigned int size, unsigned int &length);

    virtual bool validateDiskFormatBlock(unsigned char buf[], unsigned int size);
    virtual bool validateFlagsBlock(unsigned char buf[], unsigned int size);

    virtual bool validateLabelBlock(unsigned char buf[], unsigned int size);
    virtual bool validateCommentBlock(unsigned char buf[], unsigned int size);
    virtual bool validateDateBlock(unsigned char buf[], unsigned int size);
    virtual bool validateImagerBlock(unsigned char buf[], unsigned int size);
    virtual bool validateProgramBlock(unsigned char buf[], unsigned int size);

    virtual bool validateDataBlock(unsigned char buf[], unsigned int size);
    virtual bool validateTrackBlock(unsigned char buf[], unsigned int size, unsigned int &length);
    virtual bool validateSectorBlock(unsigned char buf[], unsigned int size, unsigned int &length);
    virtual bool validateRawDataBlock(unsigned char buf[], unsigned int size);
    virtual bool validateRawTrackBlock(unsigned char buf[], unsigned int size, unsigned int &length);
    virtual bool validateRawSectorBlock(unsigned char buf[], unsigned int size, unsigned int &length);

    virtual bool closeFile(void);

    virtual void disableRaw();

    // write Header
    virtual bool writeHeader();

    virtual bool setSides(unsigned char sides);
    virtual bool setTracks(unsigned char tracks);

    virtual bool writeDiskFormatBlock();
  
    virtual bool setWPParameter(bool val); 
    virtual bool setDistributionParameter(unsigned char val);
    virtual bool setTrackDataParameter(unsigned char val);

    virtual bool writeComment(unsigned char *buf, uint32_t length);

    virtual bool writeParameters();

    virtual bool startData();


    virtual bool startTrack(unsigned char side,
                           unsigned char track);
    virtual bool addSector(unsigned char sector,
                           unsigned char error,
                           unsigned char *buf,
                           uint16_t      length);
    virtual bool addRawSector(unsigned char sector, 
                              unsigned char *buf,
                              uint16_t      length);

    virtual bool endTrack();
    virtual bool endDataBlock();

    virtual bool writeRawDataBlock();

private:
    unsigned char sides_m;
    unsigned char tracks_m;
    
    unsigned char curSide_m;
    unsigned char curTrack_m;

    unsigned char sectorToIndex(unsigned char sector) { return (sector-1); }
    unsigned char indexToSector(unsigned char index) { return (index+1); }
   
    unsigned char distribution_m;
    unsigned char trackDataSource_m;
    bool          writeProtect_m;
    bool          disableRaw_m;

    std::ifstream inFile_m; 
    std::ofstream file_m; 

    unsigned int  dataSize_m;

    std::streampos  dataBlockSizePos_m, trackSizePos_m;

    bool writeBlockHeader(uint8_t blockId, uint8_t flag, uint32_t length);

//    static const unsigned char maxTracks_c  = 160; // 2 sided, 80 tracks
    static const unsigned char maxSectors_c = 10;

    std::vector<RawTrack *> rawTracks_m;
    Sector *curTrackSectors_m[maxSectors_c];

    unsigned int sectorErrs_m;
};

#endif

