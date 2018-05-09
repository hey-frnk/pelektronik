#include "MP3Display.h"

#ifndef TrackEQDISPLAY_HPP
#define TrackEQDISPLAY_HPP

typedef struct TrackEQDisplay {
  MP3Display                super;

  void        (*init)                   (struct TrackEQDisplay *this);
  void        (*setTrackInfo)            (struct TrackEQDisplay *this, int8_t *trackName, int8_t *artistName, int8_t *albumName, uint32_t length);
  void        (*changeVolume)           (struct TrackEQDisplay *this, uint8_t volume);
  void        (*changeMode)             (struct TrackEQDisplay *this, uint8_t mode);
  void        (*changeStatus)           (struct TrackEQDisplay *this, uint8_t status);

  uint8_t                   volume, mode, status, *bufLength;
  int8_t                    *trackName, *artistName, *albumName;
  uint32_t                  length;
} TrackEQDisplay;

uint8_t TrackEQDisplay_getType(void *this);
void TrackEQDisplay_show(void *this);
void TrackEQDisplay_init(TrackEQDisplay *this);

#endif
