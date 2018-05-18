#include <stdio.h>
#include "MP3Display.hpp"
#include "TrackDisplay.hpp"

uint8_t TrackDisplay::getType() {
  printf("TrackDisplay: Type: %hhu\n", this->type);
  return this->type;
}

void TrackDisplay::show() {
  printf("TrackDisplay: Show Method of TrackDisplay Called! Volume: %hhu\n", this->volume);
}

void TrackDisplay::setTrackInfo(char *trackName, char *artistName, char *albumName, uint32_t length){
  this->trackName = trackName;
  this->artistName = artistName;
  this->albumName = albumName;
  this->length = length;

  printf("TrackDisplay: setTrackInfo called!\n");
}

void TrackDisplay::changeVolume(uint8_t volume){
  this->volume = volume;
  printf("TrackDisplay: Volume change to: %hhu\n", this->volume);
}

void TrackDisplay::changeMode(uint8_t mode){
  this->mode = mode;
  printf("TrackDisplay: Mode change to: %hhu\n", this->mode);
}

void TrackDisplay::changeStatus(uint8_t status){
  this->status = status;
  printf("TrackDisplay: Status change to: %hhu\n", this->status);
}
