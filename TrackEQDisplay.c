#include <stdio.h>
#include "MP3Display.h"
#include "TrackEQDisplay.h"

struct MP3Display_vTable TrackEQDisplay_vS = {&TrackEQDisplay_getType, &TrackEQDisplay_show};

uint8_t TrackEQDisplay_getType(void *this) {
  printf("TrackEQDisplay: Type: %hhu\n", ((TrackEQDisplay *) this)->super.type);
  return ((TrackEQDisplay *) this)->super.type;
}

void TrackEQDisplay_show(void *this) {
  printf("TrackEQDisplay: Show Method of TrackEQDisplay Called!\n");
}

void TrackEQDisplay_setTrackInfo(TrackEQDisplay *this, char *trackName, char *artistName, char *albumName, uint32_t length){
  this->trackName = trackName;
  this->artistName = artistName;
  this->albumName = albumName;
  this->length = length;

  printf("TrackEQDisplay: setTrackInfo called!\n");
}

void TrackEQDisplay_changeVolume(TrackEQDisplay *this, uint8_t volume){
  this->volume = volume;
  printf("TrackEQDisplay: Volume change to: %hhu\n", this->volume);
}

void TrackEQDisplay_changeMode(TrackEQDisplay *this, uint8_t mode){
  this->mode = mode;
  printf("TrackEQDisplay: Mode change to: %hhu\n", this->mode);
}

void TrackEQDisplay_changeStatus(TrackEQDisplay *this, uint8_t status){
  this->status = status;
  printf("TrackEQDisplay: Status change to: %hhu\n", this->status);
}

void TrackEQDisplay_init(TrackEQDisplay *this) {
  printf("TrackEQDisplay initialized\n");
  MP3Display_init(&this->super);
  this->super.virtual = &TrackEQDisplay_vS;
  this->super.type = MDISPLAY_TYPE_MUSICEQPLAY;
  this->setTrackInfo = TrackEQDisplay_setTrackInfo;
  this->changeVolume = TrackEQDisplay_changeVolume;
  this->changeMode = TrackEQDisplay_changeMode;
  this->changeStatus = TrackEQDisplay_changeStatus;
}
