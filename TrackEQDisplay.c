#include <stdio.h>
#include "MP3Display.h"
#include "TrackEQDisplay.h"

struct MP3Display_vTable TrackEQDisplay_vS = {&TrackEQDisplay_getType, &TrackEQDisplay_show};

uint8_t TrackEQDisplay_getType(void *iptr) {
  printf("TrackEQDisplay: Type: %hhu\n", ((TrackEQDisplay *) iptr)->super.type);
  return ((TrackEQDisplay *) iptr)->super.type;
}

void TrackEQDisplay_show(void *iptr) {
  printf("TrackEQDisplay: Show Method of TrackEQDisplay Called!\n");
}

void TrackEQDisplay_setTrackInfo(TrackEQDisplay *iptr, char *trackName, char *artistName, char *albumName, uint32_t length){
  iptr->trackName = trackName;
  iptr->artistName = artistName;
  iptr->albumName = albumName;
  iptr->length = length;

  printf("TrackEQDisplay: setTrackInfo called!\n");
}

void TrackEQDisplay_changeVolume(TrackEQDisplay *iptr, uint8_t volume){
  iptr->volume = volume;
  printf("TrackEQDisplay: Volume change to: %hhu\n", iptr->volume);
}

void TrackEQDisplay_changeMode(TrackEQDisplay *iptr, uint8_t mode){
  iptr->mode = mode;
  printf("TrackEQDisplay: Mode change to: %hhu\n", iptr->mode);
}

void TrackEQDisplay_changeStatus(TrackEQDisplay *iptr, uint8_t status){
  iptr->status = status;
  printf("TrackEQDisplay: Status change to: %hhu\n", iptr->status);
}

void TrackEQDisplay_init(TrackEQDisplay *iptr) {
  printf("TrackEQDisplay initialized\n");
  MP3Display_init(&iptr->super);
  iptr->super.vt = &TrackEQDisplay_vS;
  iptr->super.type = MDISPLAY_TYPE_MUSICEQPLAY;
  iptr->setTrackInfo = TrackEQDisplay_setTrackInfo;
  iptr->changeVolume = TrackEQDisplay_changeVolume;
  iptr->changeMode = TrackEQDisplay_changeMode;
  iptr->changeStatus = TrackEQDisplay_changeStatus;
}
