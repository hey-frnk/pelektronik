// This is loosely based on MPAHeaderInfo (MPEG Audio Frame Header) by Konrad Windszus
// Ported to sort-of-C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "read_mheader.h"

#ifdef __arm__  // for an embedded enviroment, using FatFs from chan
#include "../fat/ff.h"				// FAT File System Library
#include "../fat/diskio.h" 			// Disk IO Initialize SPI Mutex
#define file_seek_absolute(file,position) f_lseek(file, position)
#define file_seek_relative(fi,pos) f_lseek(fi,fi->fptr+pos)
#define file_read(f,str,l,rea) f_read(f,str,(l),&(rea))
#else
//#include <iostream>
#define file_seek_absolute(file,position) fseek (file , position , SEEK_SET)
#define file_seek_relative(fi,pos) fseek(fi,pos,SEEK_CUR)
#define file_read(f,str,l,rea) rea=fread(str,1,l,f)
#endif

#define ID3HEADER_SIZE 10

// Very dirty global header compare variable
static uint32_t _mheader_hCMP = 0;

// Constant lookup tables
// sampling rates in hertz: 1. index = MPEG Version ID, 2. index = sampling rate index
const uint16_t _mh_SamplingRates[4][3] =
{
	{11025, 12000, 8000,  },	// MPEG 2.5
	{0,     0,     0,     },	// reserved
	{22050, 24000, 16000, },	// MPEG 2
	{44100, 48000, 32000  }		// MPEG 1
};

// bitrates: 1. index = LSF, 2. index = Layer, 3. index = bitrate index
const uint16_t _mh_Bitrates[2][3][15] =
{
	{	// MPEG 1
		{0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448,},	// Layer1
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384,},	// Layer2
		{0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320,}	// Layer3
	},
	{	// MPEG 2, 2.5
		{0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256,},		// Layer1
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160,},			// Layer2
		{0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160,}			// Layer3
	}
};

// allowed combination of bitrate (1.index) and mono (2.index)
/*const uint8_t _mh_AllowedModes[15][2] =
{
	// {stereo, intensity stereo, dual channel allowed,single channel allowed}
	{1, 1},		// free mode
	{0, 1},		// 32
	{0, 1},		// 48
	{0, 1},		// 56
	{1, 1},		// 64
	{0, 1},		// 80
	{1, 1},		// 96
	{1, 1},		// 112
	{1, 1},		// 128
	{1, 1},		// 160
	{1, 1},		// 192
	{1, 0},		// 224
	{1, 0},		// 256
	{1, 0},		// 320
	{1, 0}		// 384
};*/

// Samples per Frame: 1. index = LSF, 2. index = Layer
const uint16_t _mh_SamplesPerFrames[2][3] =
{
	{	// MPEG 1
		384,	// Layer1
		1152,	// Layer2
		1152	// Layer3
	},
	{	// MPEG 2, 2.5
		384,	// Layer1
		1152,	// Layer2
		576		// Layer3
	}
};

// Samples per Frame / 8
const uint8_t _mh_Coefficients[2][3] =
{
	{	// MPEG 1
		12,		// Layer1	(must be multiplied with 4, because of slot size)
		144,	// Layer2
		144		// Layer3
	},
	{	// MPEG 2, 2.5
		12,		// Layer1	(must be multiplied with 4, because of slot size)
		144,	// Layer2
		72		// Layer3
	}
};

// slot size per layer
const uint8_t _mh_SlotSizes[3] =
{
	2,			// Layer1
	0,			// Layer2
	0			// Layer3
};

// size of side information (only for Layer III)
// 1. index = LSF, 2. index = mono
/*const uint32_t _mh_SideInfoSizes[2][2] =
{
	// MPEG 1
	{32,17},
	// MPEG 2/2.5
	{17,9}
};*/

// tolerance range, look at expected offset +/- m_dwTolerance/2 for beginning of a frame
// const uint32_t _mh_Tolerance = 6;	// +/-3 bytes

// max. range where to look for frame sync
// const uint32_t _mh_MaxRange = 16384;


uint32_t _mheader_initFast(FIL* file, uint32_t fPos) {
	uint8_t header[4] = {0};
  uint32_t bytesRead = 0;
  file_seek_absolute(file, fPos);
  file_read(file, header, 4, bytesRead);
	// _mheader_hCMP = *(uint32_t *)header;
	return *(uint32_t *)header;
}

MH_STATE _mheader_initCheck(mheader *headerInstance, uint32_t fHeader) {
	// printf("InitCheck called\n");
  uint8_t *header = (uint8_t *)&fHeader;

  // Frame check, cheater version (assume little endian)
  register uint16_t hb = (*(uint16_t *)header) & 0xF0FF;
  if(!(hb == 0xF0FF || hb == 0xE0FF)) return MH_STATE_HEADERCORRUPT;

  // Fast corrupt handler
  register uint8_t _mh_version = ((header[1] >> 3) & 0x03);
  if(_mh_version == MPEGReserved) return MH_STATE_HEADERCORRUPT;

  register uint8_t _bitrateIndex = ((header[2] >> 4) & 0x0F);
  if(_bitrateIndex == 0x0F) return MH_STATE_HEADERCORRUPT;

  register uint8_t _bIndex = ((header[2] >> 2) & 0x03);
  if(_bIndex == 0x03) return MH_STATE_HEADERCORRUPT;

  // uint8_t _mh_emphasis = header[3] & 0x03;
	// if(_mh_emphasis == EmphReserved) return MH_STATE_HEADERCORRUPT;
	if((header[3] & 0x03) == EmphReserved) return MH_STATE_HEADERCORRUPT;

  uint8_t _mh_layer = (3 - ((header[1] >> 1) & 0x03));
  if(_mh_layer == LayerReserved) return MH_STATE_HEADERCORRUPT;

  headerInstance->mh_version = _mh_version;
  // if(headerInstance->mh_version == MPEGReserved) return MH_STATE_HEADERCORRUPT;
  uint8_t _mh_lowerSamplingFrequencies = (_mh_version == MPEG1) ? 0 : 1;

  headerInstance->mh_bitrate = _mh_Bitrates[_mh_lowerSamplingFrequencies][_mh_layer][_bitrateIndex] * 1000; // kbit -> bit
  if(headerInstance->mh_bitrate == 0) return MH_STATE_FREEBITRATE;

  headerInstance->mh_layer = _mh_layer;
  headerInstance->_mh_lowerSamplingFrequencies = _mh_lowerSamplingFrequencies;
  headerInstance->mh_samplesPerSec = _mh_SamplingRates[_mh_version][_bIndex];

  // padding bit, bit 22
  headerInstance->mh_paddingSize = ((header[2] >> 1) & 0x01);

  headerInstance->mh_samplesPerFrame = _mh_SamplesPerFrames[_mh_lowerSamplingFrequencies][_mh_layer];

  _mheader_hCMP = *(uint32_t *)header; // Update new header compare
  return MH_STATE_OK;
}

// Read in and validate header
MH_STATE mheader_init(mheader *headerInstance, FIL* file, uint32_t fPos) {
	// printf("Init called with: %u\n", fPos);

  uint8_t header[4] = {0};
  uint32_t bytesRead = 0;
  file_seek_absolute(file, fPos);
  file_read(file, header, 4, bytesRead);

  // Frame check, cheater version (assume little endian)
  register uint16_t hb = (*(uint16_t *)header) & 0xF0FF;
  if(!(hb == 0xF0FF || hb == 0xE0FF)) return MH_STATE_HEADERCORRUPT;

  // register uint8_t _n1 = *(header + 1) >> 4;
  // if(!((*header == 0xff) && (_n1 == 0x0F || (_n1 == 0x0E)))) return MH_STATE_HEADERCORRUPT;

  // Fast corrupt handler
  register uint8_t _mh_version = ((header[1] >> 3) & 0x03);
  if(_mh_version == MPEGReserved) return MH_STATE_HEADERCORRUPT;

  register uint8_t _bitrateIndex = ((header[2] >> 4) & 0x0F);
  if(_bitrateIndex == 0x0F) return MH_STATE_HEADERCORRUPT;

  register uint8_t _bIndex = ((header[2] >> 2) & 0x03);
  if(_bIndex == 0x03) return MH_STATE_HEADERCORRUPT;

  // uint8_t _mh_emphasis = header[3] & 0x03;
	// if(_mh_emphasis == EmphReserved) return MH_STATE_HEADERCORRUPT;
	if((header[3] & 0x03) == EmphReserved) return MH_STATE_HEADERCORRUPT;

  uint8_t _mh_layer = (3 - ((header[1] >> 1) & 0x03));
  if(_mh_layer == LayerReserved) return MH_STATE_HEADERCORRUPT;

  // MPEG Version, bit 11, 12


  headerInstance->mh_version = _mh_version;
  // if(headerInstance->mh_version == MPEGReserved) return MH_STATE_HEADERCORRUPT;
  uint8_t _mh_lowerSamplingFrequencies = (_mh_version == MPEG1) ? 0 : 1;

  // Get Layer, bit 13, 14
  // if(headerInstance->mh_layer == LayerReserved) return MH_STATE_HEADERCORRUPT;

  // Protection bit (inverted), bit 15, dont care
  // headerInstance->mh_CRC = !((header[1]) & 0x01);

  // Retrieve Bitrate
  // uint8_t _bitrateIndex = ((header[2] >> 4) & 0x0F);
  // if(_bitrateIndex == 0x0F) return MH_STATE_HEADERCORRUPT;
  headerInstance->mh_bitrate = _mh_Bitrates[_mh_lowerSamplingFrequencies][_mh_layer][_bitrateIndex] * 1000; // kbit -> bit
  if(headerInstance->mh_bitrate == 0) return MH_STATE_FREEBITRATE;

  headerInstance->mh_layer = _mh_layer;
  headerInstance->_mh_lowerSamplingFrequencies = _mh_lowerSamplingFrequencies;

  // Retrieve sampling rate, bit 20, 21
  // uint8_t _bIndex = ((header[2] >> 2) & 0x03);
  // if(_bIndex == 0x03) return MH_STATE_HEADERCORRUPT;
  headerInstance->mh_samplesPerSec = _mh_SamplingRates[_mh_version][_bIndex];

  // padding bit, bit 22
  headerInstance->mh_paddingSize = ((header[2] >> 1) & 0x01);

  headerInstance->mh_samplesPerFrame = _mh_SamplesPerFrames[_mh_lowerSamplingFrequencies][_mh_layer];

  // private bit, bit 23, dont care
  // headerInstance->mh_privated = (header[2]) & 0x01;

  // channel mode, bit 24, 25
  // headerInstance->mh_channelMode = ((header[3] >> 6) & 0x03);

  // mode extension
  // headerInstance->mh_modeExt = ((header[3] >> 4) & 0x03);

  // determine the bound for intensity stereo, dont care
	// if (headerInstance->mh_channelMode == JointStereo)
		// headerInstance->mh_bound = 4 + (headerInstance->mh_modeExt << 2);

  // copyright bit, bit 28, dont care
  // headerInstance->mh_copyrighted = (header[3] >> 3) & 0x01;

  // original bit, bit 29, dont care
	// headerInstance->mh_original = (header[3] >> 2) & 0x01;

  // emphasis, bit 30, 31
	// headerInstance->mh_emphasis = _mh_emphasis;
	// if(headerInstance->mh_emphasis == EmphReserved) return MH_STATE_HEADERCORRUPT;

  // extended check for Layer II
	/*if (headerInstance->mh_layer == Layer2) {
		// MPEG 1
		if (headerInstance->mh_version == MPEG1) {
			if (!_mh_AllowedModes[_bitrateIndex][mheader_isMono(headerInstance)]) return MH_STATE_HEADERCORRUPT;

			// which allocation table is used
			switch (headerInstance->mh_bitrate / 1000 / (mheader_isMono(headerInstance) ? 1 : 2)) {
				case 32:
				case 48:
					if (headerInstance->mh_samplesPerSec == 32000) headerInstance->mh_allocationTableIndex = 3;	// table d
					else headerInstance->mh_allocationTableIndex = 2;	// table c
					break;
				case 56:
				case 64:
				case 80:
					if (headerInstance->mh_samplesPerSec != 48000) {
						headerInstance->mh_allocationTableIndex = 0;	// table a
						break;
					}
				case 96:
				case 112:
				case 128:
				case 160:
				case 192:
					if (headerInstance->mh_samplesPerSec != 48000) {
						headerInstance->mh_allocationTableIndex = 1;	// table b
						break;
					}
					else headerInstance->mh_allocationTableIndex = 0;	// table a
					break;
			}
		}
		else	// MPEG 2/2.5
			headerInstance->mh_allocationTableIndex = 4;
	}*/


  _mheader_hCMP = *(uint32_t *)header; // Update new header compare
  return MH_STATE_OK;
}

// Mono?
// uint8_t mheader_isMono(mheader *headerInstance) {return(headerInstance->mh_channelMode == SingleChannel) ? 1 : 0;};

// Get Total Length
uint32_t _mheader_getLength(mheader *headerInstance, uint32_t numFrames) {return numFrames * headerInstance->mh_samplesPerFrame / headerInstance->mh_samplesPerSec;};

uint32_t mheader_getLength(FIL* file) {
  uint32_t firstFrameOffset = mheader_getFirstFrameOffset(file);
  uint32_t totalFrameCount = mheader_getTotalFrameCount(file, firstFrameOffset);

  mheader h;
  mheader_init(&h, file, firstFrameOffset);

  return _mheader_getLength(&h, totalFrameCount);
}

uint32_t j = 0;

// Get frame size
uint32_t mheader_getFrameSize(mheader *headerInstance){
  // printf("GetFrameSize called! %u\n", ++j);

  return (uint16_t)(\
        (_mh_Coefficients[headerInstance->_mh_lowerSamplingFrequencies][headerInstance->mh_layer] * headerInstance->mh_bitrate / headerInstance->mh_samplesPerSec) \
        + headerInstance->mh_paddingSize) << _mh_SlotSizes[headerInstance->mh_layer];};

// Get first frame
uint32_t mheader_getFirstFrameOffset(FIL *file) {
  uint32_t i = 0;
  uint8_t* idheader = (uint8_t *)malloc(10 * sizeof(uint8_t));
  uint32_t bytesRead = 0;
  // Navigate to file zero
  file_seek_absolute(file, 0);
  file_read(file, idheader, ID3HEADER_SIZE, bytesRead);
  if ((idheader[0] != 'I') || (idheader[1] != 'D') || (idheader[2] != '3')) return 0;
  // Skip id3 header
  uint32_t _id3hSize = (idheader[6] << 21) | (idheader[7] << 14) | (idheader[8] << 7) | idheader[9];
  // printf("id3hsize %u\n", _id3hSize);

  // Retrieve first frame
  i += _id3hSize;
  mheader h;
  while(i++)
    if(mheader_init(&h, file, i) == MH_STATE_OK) break;

  free(idheader);
  return i;
}

uint32_t mheader_getTotalFrameCount(FIL *file, uint32_t fPos) {
  // Assume valid header pointer
  mheader h;
  if(mheader_init(&h, file, fPos) != MH_STATE_OK) return 0;
  uint32_t cnt = 1, nextHSize = mheader_getFrameSize(&h);

  while(1) {
    // Here is the old frame size
    uint32_t _hCMPb = _mheader_hCMP;
    fPos += nextHSize;

		// Exact same header
		uint32_t init = _mheader_initFast(file, fPos);
		if(init == _hCMPb){	// If new header is the same header, we don't need to set the struct instance
			++cnt;						// Increase only, done
			continue;
		} else {						// Otherwise
			if(_mheader_initCheck(&h, init) == MH_STATE_OK) ++cnt;		// Check if it is a valid header
			else return cnt;																					// otherwise we're done counting
			if(_hCMPb != _mheader_hCMP) nextHSize = mheader_getFrameSize(&h);		// Header has different size?
		}
  }
  return 0;   // Oops
}






//
