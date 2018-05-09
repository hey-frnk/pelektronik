#include "MP3Display.h"

#ifndef TrackDISPLAY_HPP
#define TrackDISPLAY_HPP

typedef struct TrackDisplay {
  MP3Display                super;

  void        (*init)                   (struct TrackDisplay *this);
  void        (*setTrackInfo)            (struct TrackDisplay *this, int8_t *trackName, int8_t *artistName, int8_t *albumName, uint32_t length);
  void        (*changeVolume)           (struct TrackDisplay *this, uint8_t volume);
  void        (*changeMode)             (struct TrackDisplay *this, uint8_t mode);
  void        (*changeStatus)           (struct TrackDisplay *this, uint8_t status);

  uint8_t                   volume, mode, status, *bufLength;
  int8_t                    *trackName, *artistName, *albumName;
  uint32_t                  length;
} TrackDisplay;

uint8_t TrackDisplay_getType(void *this);
void TrackDisplay_show(void *this);
void TrackDisplay_init(TrackDisplay *this);

#endif
