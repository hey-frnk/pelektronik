#ifndef _READ_MHEADER
#define _READ_MHEADER

#include <stdint.h>

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
  uint32_t        mh_samplesPerSec,
                  mh_samplesPerFrame,
                  mh_bitrate,                // In bit per second
                  mh_paddingSize;
  // uint16_t        // mh_bound,
                  // mh_allocationTableIndex;
  uint8_t         _mh_lowerSamplingFrequencies;
                  // mh_copyrighted,
                  // mh_privated,
                  // mh_original,
                  // mh_CRC,
                  // mh_modeExt;

  uint8_t         mh_version,
                  mh_layer;
                  // mh_emphasis,
                  // mh_channelMode;
} mheader;



// Read in MPEG header
MH_STATE mheader_init(mheader *headerInstance, uint8_t *header);

// Mono?
uint8_t mheader_isMono(mheader *headerInstance);

// Get total length
uint32_t mheader_getLength(uint8_t *header);

// Get frame size
uint32_t mheader_getFrameSize(mheader *headerInstance);

// Calculate first frame
uint32_t mheader_getFirstFrameOffset(uint8_t *header);

// Calculate total frames
uint32_t mheader_getTotalFrameCount(uint8_t *header);

#endif
