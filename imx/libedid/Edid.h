#ifndef _NXP_EDID_H_
#define _NXP_EDID_H_

#define EDID_LENGTH 128
#define CEA_EXTENSION   0x02
#define DATA_BLOCK_EXTENDED_TAG     0x07
#define HDR_STATIC_METADATA_BLOCK   0x06
#define SDR_DEFAULT_LUMINANCE 100

#define EXTENSION_NUM   0x7e
#define LEN_MASK    0x1f
#define TAG_SHIFT   5

struct HdrMetaData {
    float max_cll;
    float max_fall;
    float min_cll;
};

class Edid
{
public:
    Edid(int fd,uint32_t connectorID);
    virtual ~Edid();

    //check HDR is supported or not
    bool isHdrSupported();

    //get EdidMetaData
    int getHdrMetaData(HdrMetaData* hdrMetaData);

private:
    bool mIsHdrSupported;
    HdrMetaData mHdrMetaData;

    //get edid data from drm and parse it
    void getEdidDataFromDrm(int fd,uint32_t connectorID);

    //check whether the edid raw data is valid or not
    bool isEdidValid(unsigned char *edid);

    //get cea extension edid raw data
    unsigned char* getCeaExtensionData(unsigned char *edid);

    //return cea data block length
    int getDataBlockLen(unsigned char *db);

    //check whether given data block is hdr metadata or not
    bool isHdrMetadataBlock(unsigned char *db);

    //parse the given hdr data block
    void parseHdrMetadataBlock(unsigned char *db);

    //parse cea extension edid raw data
    void parseCeaExtData(unsigned char *edid_ext);

    //dump EdidMetaData
    void dumpHdrMetaData();
};
#endif
