#ifdef __cplusplus
extern "C" {
#endif

#include "MP3Display.h"

#ifndef TrackDISPLAY_HPP
#define TrackDISPLAY_HPP

typedef struct TrackDisplay {
  MP3Display                super;

  void        (*init)                   (struct TrackDisplay *iptr);
  void        (*setTrackInfo)           (struct TrackDisplay *iptr, char *trackName, char *artistName, char *albumName, uint32_t length);
  void        (*changeVolume)           (struct TrackDisplay *iptr, uint8_t volume);
  void        (*changeMode)             (struct TrackDisplay *iptr, uint8_t mode);
  void        (*changeStatus)           (struct TrackDisplay *iptr, uint8_t status);

  uint8_t                   volume, mode, status, *bufLength;
  char                      *trackName, *artistName, *albumName;
  uint32_t                  length;
} TrackDisplay;

uint8_t TrackDisplay_getType(void *iptr);
void TrackDisplay_show(void *iptr);
void TrackDisplay_init(TrackDisplay *iptr);

#endif

#ifdef __cplusplus
}
#endif
