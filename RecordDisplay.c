/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in July 2018
 *      Author: Copyright (C) Frank Zheng
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MP3Display.h"
#include "RecordDisplay.h"

#ifndef DEBUG
#include "st7735.h"
#else
#include <time.h>
#endif

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"
#include "RTCI.h"

#define VISUALIZEPOSITION 50
#define VBAR_LENGTH   125

struct MP3Display_vTable RecordDisplay_vS = {&RecordDisplay_getType, &RecordDisplay_show};

uint8_t _RecordDisplay_getIntensity(RecordDisplay *iptr, uint8_t indexPos) {
  /* return the intensity value that is stored in the 16 bit intensity memory bits
  align: i3, i2, i1, i0 in every half word. To go to the correct position, do indexPos / 4;
  then shift the desired bit by 4 * (indexPos % 4) and extract with 0xF */
  return ((iptr->intensityDisplay[indexPos >> 2]) >> ((indexPos & 0x03) << 2)) & 0x000F;
}

void _RecordDisplay_setIntensity(RecordDisplay *iptr, uint8_t indexPos, uint8_t intensity) {
  iptr->intensityDisplay[indexPos >> 2] |= intensity << ((indexPos & 0x03) << 2);
}

uint8_t RecordDisplay_getType(void *iptr) {
  #ifdef DEBUG
  printf("RecordDisplay: Type: %hhu\n", ((RecordDisplay *) iptr)->super.type);
  #endif
  return ((RecordDisplay *) iptr)->super.type;
}

void RecordDisplay_show(void *iptr) {
  mdisplay_hlvf_DrawIcon(138, 90, NAV_SOUND, COLOR_GRAY);
  mdisplay_hlvf_DrawRectangle(8, VISUALIZEPOSITION, 144, 32, COLOR_LGRAY);
  mdisplay_hlvf_DrawColorWheelStringFast(_global_width >> 1, VISUALIZEPOSITION - 15, "Voice Recorder", FONT_8X12, ALIGNMENT_CENTER);

  RecordDisplay *tptr = ((RecordDisplay *) iptr);
  #ifdef DEBUG
  printf("RecordDisplay: Show Method of RecordDisplay Called!\n");
  #endif

  uint16_t volume = 0;

  #ifdef DEBUG
  srand(time(NULL));
  volume = rand() % 4096;
  #else
  volume = 647;
  #endif

  // Volume bar
  uint8_t volumeBar = volume * ((float)VBAR_LENGTH / 4096);
  mdisplay_hlvf_FillRectangle(8, 95, VBAR_LENGTH, 4, 0xe73c);
  mdisplay_hlvf_FillRectangle(8, 95, volumeBar, 4, COLOR_GRAY);

  if(tptr->recording) {
    uint8_t intensity = 0;

    #ifdef DEBUG
    srand(time(NULL));
    intensity = rand() % 16;
    #else
    intensity = 14; // lol
    #endif

    intensity <<=1; // Resolution cheating

    _RecordDisplay_setIntensity(tptr, tptr->intensityPos, intensity);
    if(tptr->intensityPos < 144) {
      #ifdef DEBUG
      mdisplay_hlvf_DrawRectangle(8 + tptr->intensityPos, VISUALIZEPOSITION + 16 - (intensity >> 1), 1, intensity, COLOR_DARKBLUE);
      #else
      st7735_DrawVLine(COLOR_DARKBLUE, 8 + tptr->intensityPos, VISUALIZEPOSITION + 16 - (intensity >> 1), intensity);
      #endif
    }

    ++tptr->intensityPos;
  }
}

void RecordDisplay_startRecording(RecordDisplay *iptr) {
  #ifdef DEBUG
  printf("RecordDisplay: Timer Started!\n");
  #endif
  mdisplay_hlvf_FillRectangle(9, VISUALIZEPOSITION + 1, 142, 30, COLOR_WHITE);
  TIMEI_startTimer();
  iptr->recording = 1;
}

void RecordDisplay_stopRecording(RecordDisplay *iptr) {
  #ifdef DEBUG
  printf("RecordDisplay: Timer Stopped!\n");
  #endif
  TIMEI_stopAndResetTimer();
  iptr->recording = 0;
}

void RecordDisplay_createRecordFile(RecordDisplay *iptr, char *fileName) {

}

void RecordDisplay_init(RecordDisplay *iptr) {
  #ifdef DEBUG
  printf("RecordDisplay initialized\n");
  #endif
  MP3Display_init(&iptr->super);
  iptr->super.vt = &RecordDisplay_vS;
  iptr->super.type = MDISPLAY_TYPE_VOICERECORD;
  iptr->startRecording = RecordDisplay_startRecording;
  iptr->stopRecording = RecordDisplay_stopRecording;
  iptr->createRecordFile = RecordDisplay_createRecordFile;

  iptr->recording = 0;
  iptr->fileName = NULL;
  iptr->intensityPos = 0;
  iptr->intensityDisplay = (uint16_t *)malloc(36 * sizeof(uint16_t));

  // Wipe screen
  mdisplay_hlvf_FillScreen(COLOR_WHITE);

  // Init drawing
  mdisplay_hlvf_DrawString(_global_width >> 1, VISUALIZEPOSITION + 16, "Play to start recording", COLOR_LGRAY, FONT_5X7, ALIGNMENT_CENTER);
}

void RecordDisplay_deInit(RecordDisplay *iptr) {
  free(iptr->intensityDisplay);
}
