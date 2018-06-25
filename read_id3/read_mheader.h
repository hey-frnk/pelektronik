#ifndef _READ_MHEADER
#define _READ_MHEADER


#ifdef __arm__
#include "../ffs/ff.h"				// FAT File System Library
#else  // for an embedded enviroment, using FatFs from chan
#define FIL FILE
#define rprintf printf
#endif


typedef enum {
  MH_STATE_OK = 0,
  MH_STATE_HEADERCORRUPT,
  MH_STATE_FREEBITRATE,     // unsupported
} MH_STATE;

enum mh_VERSION{
  MPEG25 = 0,
  MPEGReserved,
  MPEG2,
  MPEG1
};

enum mh_LAYER{
  Layer1,
  Layer2,
  Layer3,
  LayerReserved
};

enum mh_EMPHASIS{
  EmphNone = 0,
  Emph5015,
  EmphReserved,
  EmphCCITJ17
};

enum mh_CHANNELMODE{
  Stereo,
  JointStereo,
  DualChannel,
  SingleChannel
};

typedef struct mheader{
  uint16_t        mh_samplesPerSec,
                  mh_samplesPerFrame;
  uint32_t        mh_bitrate;                // In bit per second
                  // mh_bound,
                  // mh_allocationTableIndex;
  uint8_t         mh_paddingSize,
                  _mh_lowerSamplingFrequencies,
                  // mh_copyrighted,
                  // mh_privated,
                  // mh_original,
                  // mh_CRC,
                  // mh_modeExt;

                  mh_version,
                  mh_layer;
                  // mh_emphasis,
                  // mh_channelMode;
} mheader;



// Read in MPEG header
MH_STATE mheader_init(mheader *headerInstance, FIL* file, uint32_t fPos);

// Mono?
uint8_t mheader_isMono(mheader *headerInstance);

// Get total length
uint32_t mheader_getLength(FIL* file);
uint32_t _mheader_getLength(mheader *headerInstance, uint32_t numFrames); // Internal

// Get frame size
uint32_t mheader_getFrameSize(mheader *headerInstance);

// Calculate first frame
uint32_t mheader_getFirstFrameOffset(FIL *file);

// Calculate total frames
uint32_t mheader_getTotalFrameCount(FIL *file, uint32_t fPos);

#endif
