#ifdef __cplusplus
extern "C" {
#endif

#include "MP3Display.h"

#ifndef TrackEQDISPLAY_HPP
#define TrackEQDISPLAY_HPP

typedef struct TrackEQDisplay {
  MP3Display                super;

  void        (*init)                   (struct TrackEQDisplay *iptr);
  void        (*setTrackInfo)           (struct TrackEQDisplay *iptr, char *trackName, char *artistName, char *albumName, uint32_t length);
  void        (*changeVolume)           (struct TrackEQDisplay *iptr, uint8_t volume);
  void        (*changeMode)             (struct TrackEQDisplay *iptr, uint8_t mode);
  void        (*changeStatus)           (struct TrackEQDisplay *iptr, uint8_t status);

  uint8_t                   volume, mode, status, *bufLength;
  char                      *trackName, *artistName, *albumName;
  uint32_t                  length;
} TrackEQDisplay;

uint8_t TrackEQDisplay_getType(void *iptr);
void TrackEQDisplay_show(void *iptr);
void TrackEQDisplay_init(TrackEQDisplay *iptr);

#endif

#ifdef __cplusplus
}
#endif
