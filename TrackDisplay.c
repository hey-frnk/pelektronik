/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MP3Display.h"
#include "TrackDisplay.h"

#ifndef DEBUG
#include "st7735.h"
#endif

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"

#define PBAR_LENGTH   (160 - 60)

struct MP3Display_vTable TrackDisplay_vS = {&TrackDisplay_getType, &TrackDisplay_show};

char *_concat(const char *s1, const char *s2){
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

uint8_t TrackDisplay_getType(void *iptr) {
  #ifdef DEBUG
  printf("TrackDisplay: Type: %hhu\n", ((TrackDisplay *) iptr)->super.type);
  #endif
  return ((TrackDisplay *) iptr)->super.type;
}

void TrackDisplay_show(void *iptr) {
  TrackDisplay *tptr = ((TrackDisplay *) iptr);
  // printf("TrackDisplay: Show Method of MP3Display Called! Volume: %hhu\n", tptr->volume);

  // Retrieve track name text width, retrieve artist, album information
  uint16_t _trNWidth = strlen(tptr->trackName);
  char* _BUF0 = _concat(" - ", tptr->albumName);
  char* _BUF1 = _concat(tptr->artistName, _BUF0);
  uint16_t _aaWidth = strlen(_BUF1);

  uint8_t _blankFlag0 = (_trNWidth << 3) <= (_global_width - 20);
  uint8_t _blankFlag1 = (_aaWidth * 6) <= (_global_width - 20);

  if(_blankFlag0){
    // No rotating required
    // mdisplay_hlvf_DrawColorWheelString((_global_width >> 1), 45, tptr->trackName, 0, 255, 153, 77, FONT_8X12, ALIGNMENT_CENTER);
    mdisplay_hlvf_DrawColorWheelStringFast((_global_width >> 1), 40, tptr->trackName, FONT_8X12, ALIGNMENT_CENTER);
  } else {
    // Rotate!
    const uint16_t _TNBUFLENGTH = 19;
    static uint16_t rPos = 0, dx = 0; // Begin very left
    if(dx < 7) ++dx;
    else{
      dx = 0;
      rPos = (rPos + (_TNBUFLENGTH - 1) < _trNWidth) ? rPos + 1 : 0;
    }

    char _TNBUF[_TNBUFLENGTH];            // 17 chars + 1 + one terminator
    strncpy(_TNBUF, tptr->trackName + rPos, (_TNBUFLENGTH - 1));
    _TNBUF[(_TNBUFLENGTH - 1)] = '\0';

    mdisplay_hlvf_FillRectangle(10, 40, _global_width - 20, 12, COLOR_WHITE);
    // mdisplay_hlvf_DrawColorWheelString(10 - dx, 45, _TNBUF, 0, 255, 153, 77, FONT_8X12, ALIGNMENT_LEFT);
    mdisplay_hlvf_DrawColorWheelStringFast(10 - dx, 40, _TNBUF, FONT_8X12, ALIGNMENT_LEFT);
    mdisplay_hlvf_FillRectangle(0, 40, 10, 12, COLOR_WHITE);
    mdisplay_hlvf_FillRectangle(_global_width - 10, 40, 10, 12, COLOR_WHITE);
  }

  if(_blankFlag1){ // No rotating, too
    mdisplay_hlvf_DrawString((_global_width >> 1), 55, _BUF1, COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
  } else {
    // Rotate!
    const uint16_t _AALENGTH = 25;
    static uint16_t rPos = 0, dx = 0; // Begin very left
    if(dx < 5) ++dx;
    else{
      dx = 0;
      rPos = (rPos + (_AALENGTH - 1) < _aaWidth) ? rPos + 1 : 0;
    }

    char _AABUF[_AALENGTH];            // 17 chars + 1 + one terminator
    strncpy(_AABUF, _BUF1 + rPos, (_AALENGTH - 1));
    _AABUF[(_AALENGTH - 1)] = '\0';

	  mdisplay_hlvf_FillRectangle(10, 55, _global_width - 20, 7, COLOR_WHITE);
    mdisplay_hlvf_DrawString(10 - dx, 55, _AABUF, COLOR_GRAY, FONT_5X7, ALIGNMENT_LEFT);
    mdisplay_hlvf_FillRectangle(0, 55, 10, 7, COLOR_WHITE);
    mdisplay_hlvf_FillRectangle(_global_width - 10, 55, 9, 7, COLOR_WHITE);
    // HAL_Delay(100);
  }

  free(_BUF0); free(_BUF1);

  // Progress bar updating
  uint32_t songProgress = TIMEI_getSongProgress();
  uint8_t progress = ((songProgress <= tptr->length) ? songProgress : 0) * ((float)PBAR_LENGTH / tptr->length);

  // Song play end, toggle interrupt state
  if(progress == PBAR_LENGTH) tptr->changeStatus(tptr, TRACKDISPLAY_STATUS_ENDED);

  mdisplay_hlvf_FillRectangle(10, 75, PBAR_LENGTH, 4, 0xe73c);
  mdisplay_hlvf_FillRectangle(10, 75, progress, 4, COLOR_GRAY);
  // Get minute second representation
  uint32_t lMinutes = tptr->length / 60, lSeconds = tptr->length % 60;
  char tConst[6] = {lMinutes / 10 + '0', lMinutes % 10 + '0', ':', lSeconds / 10 + '0', lSeconds % 10 + '0', 0};
  mdisplay_hlvf_DrawString(_global_width - 10, 72, tConst, COLOR_GRAY, FONT_5X7, ALIGNMENT_RIGHT);
}

void TrackDisplay_setTrackInfo(TrackDisplay *iptr, char *trackName, char *artistName, char *albumName, uint32_t length){
  iptr->trackName = trackName;
  iptr->artistName = artistName;
  iptr->albumName = albumName;
  iptr->length = length;

  #ifdef DEBUG
  printf("TrackDisplay: setTrackInfo called!\n");
  #endif
}

void TrackDisplay_changeVolume(TrackDisplay *iptr, uint8_t volume){
  iptr->volume = volume;
  #ifdef DEBUG
  printf("TrackDisplay: Volume change to: %hhu\n", iptr->volume);
  #endif
}

void TrackDisplay_changeMode(TrackDisplay *iptr, uint8_t mode){
  iptr->mode = mode;
  // Shuffle Status
  switch(mode){
    case TRACKDISPLAY_MODE_NONE:      mdisplay_hlvf_FillRectangle(115, 1, 16, 16, COLOR_WHITE);        break;    // Blank this
    case TRACKDISPLAY_MODE_SHUFFLE:   mdisplay_hlvf_DrawIcon(115, 1, NAV_SHUFFLE, 0xe73c);          break;
    case TRACKDISPLAY_MODE_REPEATALL: mdisplay_hlvf_DrawIcon(115, 1, NAV_REPA, 0xe73c);             break;
    case TRACKDISPLAY_MODE_REPEATONE: mdisplay_hlvf_DrawIcon(115, 1, NAV_REPO, 0xe73c);             break;
  }
  #ifdef DEBUG
  printf("TrackDisplay: Mode change to: %hhu\n", iptr->mode);
  #endif
}

void TrackDisplay_changeStatus(TrackDisplay *iptr, uint8_t status){
  iptr->status = status;

  if(iptr->status == TRACKDISPLAY_STATUS_PLAYING) mdisplay_hlvf_DrawIcon((_global_width >> 1) - 8, 88, NAV_PAUSE, COLOR_GRAY);
  else if(iptr->status == TRACKDISPLAY_STATUS_PAUSED) mdisplay_hlvf_DrawIcon((_global_width >> 1) - 8, 88, NAV_PLAY, COLOR_GRAY);

  #ifdef DEBUG
  printf("TrackDisplay: Status change to: %hhu\n", iptr->status);
  #endif
}

void TrackDisplay_init(TrackDisplay *iptr) {
  #ifdef DEBUG
  printf("TrackDisplay initialized\n");
  #endif
  MP3Display_init(&iptr->super);
  iptr->super.vt = &TrackDisplay_vS;
  iptr->super.type = MDISPLAY_TYPE_MUSICPLAY;
  iptr->setTrackInfo = TrackDisplay_setTrackInfo;
  iptr->changeVolume = TrackDisplay_changeVolume;
  iptr->changeMode = TrackDisplay_changeMode;
  iptr->changeStatus = TrackDisplay_changeStatus;

  // Wipe screen
  mdisplay_hlvf_FillScreen(COLOR_WHITE);

  iptr->changeStatus(iptr, TRACKDISPLAY_STATUS_PLAYING);
  mdisplay_hlvf_DrawIcon((_global_width >> 1) + 26, 88, NAV_FWD, COLOR_GRAY);
  mdisplay_hlvf_DrawIcon((_global_width >> 1) - 42, 88, NAV_RWD, COLOR_GRAY);
}
