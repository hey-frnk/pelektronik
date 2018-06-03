#include <stdio.h>
#include "MP3Display.h"
#include "TrackDisplay.h"

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"

struct MP3Display_vTable TrackDisplay_vS = {&TrackDisplay_getType, &TrackDisplay_show};

uint8_t TrackDisplay_getType(void *iptr) {
  printf("TrackDisplay: Type: %hhu\n", ((TrackDisplay *) iptr)->super.type);
  return ((TrackDisplay *) iptr)->super.type;
}

void TrackDisplay_show(void *iptr) {
  printf("TrackDisplay: Show Method of MP3Display Called! Volume: %hhu\n", ((TrackDisplay *) iptr)->volume);

  char str1[] = "On some nights I still believe that a car with the gas needle on empty can run about fifty more miles with the right music on the radio";

  mdisplay_hlvf_FillScreen(COLOR_WHITE);
  mdisplay_hlvf_DrawColorWheelString(1, 0, str1, 0, 255, 153, 77, 0);
}

void TrackDisplay_setTrackInfo(TrackDisplay *iptr, char *trackName, char *artistName, char *albumName, uint32_t length){
  iptr->trackName = trackName;
  iptr->artistName = artistName;
  iptr->albumName = albumName;
  iptr->length = length;

  printf("TrackDisplay: setTrackInfo called!\n");
}

void TrackDisplay_changeVolume(TrackDisplay *iptr, uint8_t volume){
  iptr->volume = volume;
  printf("TrackDisplay: Volume change to: %hhu\n", iptr->volume);
}

void TrackDisplay_changeMode(TrackDisplay *iptr, uint8_t mode){
  iptr->mode = mode;
  printf("TrackDisplay: Mode change to: %hhu\n", iptr->mode);
}

void TrackDisplay_changeStatus(TrackDisplay *iptr, uint8_t status){
  iptr->status = status;
  printf("TrackDisplay: Status change to: %hhu\n", iptr->status);
}

void TrackDisplay_init(TrackDisplay *iptr) {
  printf("TrackDisplay initialized\n");
  MP3Display_init(&iptr->super);
  iptr->super.vt = &TrackDisplay_vS;
  iptr->super.type = MDISPLAY_TYPE_MUSICPLAY;
  iptr->setTrackInfo = TrackDisplay_setTrackInfo;
  iptr->changeVolume = TrackDisplay_changeVolume;
  iptr->changeMode = TrackDisplay_changeMode;
  iptr->changeStatus = TrackDisplay_changeStatus;
}
