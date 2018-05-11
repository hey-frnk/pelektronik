#include <stdio.h>
#include "MP3Display.h"
#include "TrackDisplay.h"

struct MP3Display_vTable TrackDisplay_vS = {&TrackDisplay_getType, &TrackDisplay_show};

uint8_t TrackDisplay_getType(void *this) {
  printf("TrackDisplay: Type: %hhu\n", ((TrackDisplay *) this)->super.type);
  return ((TrackDisplay *) this)->super.type;
}

void TrackDisplay_show(void *this) {
  printf("TrackDisplay: Show Method of MP3Display Called! Volume: %hhu\n", ((TrackDisplay *) this)->volume);
}

void TrackDisplay_setTrackInfo(TrackDisplay *this, char *trackName, char *artistName, char *albumName, uint32_t length){
  this->trackName = trackName;
  this->artistName = artistName;
  this->albumName = albumName;
  this->length = length;

  printf("TrackDisplay: setTrackInfo called!\n");
}

void TrackDisplay_changeVolume(TrackDisplay *this, uint8_t volume){
  this->volume = volume;
  printf("TrackDisplay: Volume change to: %hhu\n", this->volume);
}

void TrackDisplay_changeMode(TrackDisplay *this, uint8_t mode){
  this->mode = mode;
  printf("TrackDisplay: Mode change to: %hhu\n", this->mode);
}

void TrackDisplay_changeStatus(TrackDisplay *this, uint8_t status){
  this->status = status;
  printf("TrackDisplay: Status change to: %hhu\n", this->status);
}

void TrackDisplay_init(TrackDisplay *this) {
  printf("TrackDisplay initialized\n");
  MP3Display_init(&this->super);
  this->super.virtual = &TrackDisplay_vS;
  this->super.type = MDISPLAY_TYPE_MUSICPLAY;
  this->setTrackInfo = TrackDisplay_setTrackInfo;
  this->changeVolume = TrackDisplay_changeVolume;
  this->changeMode = TrackDisplay_changeMode;
  this->changeStatus = TrackDisplay_changeStatus;
}
