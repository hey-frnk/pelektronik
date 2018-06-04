#include <stdio.h>
#include "MP3Display.h"
#include "TrackDisplay.h"

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"

struct MP3Display_vTable TrackDisplay_vS = {&TrackDisplay_getType, &TrackDisplay_show};

uint8_t TrackDisplay_getType(void *iptr) {
  printf("TrackDisplay: Type: %hhu\n", ((TrackDisplay *) iptr)->super.type);
  return ((TrackDisplay *) iptr)->super.type;
}

void TrackDisplay_show(void *iptr) {
  TrackDisplay *tptr = ((TrackDisplay *) iptr);
  printf("TrackDisplay: Show Method of MP3Display Called! Volume: %hhu\n", tptr->volume);

  // Universal header
  mdisplay_hlvf_FillScreen(COLOR_WHITE);
  mdisplay_hlvf_DrawRectangle(0, 18, ST7735_LCD_PIXEL_WIDTH, 1, 0xe73c);
  mdisplay_hlvf_DrawIcon(115, 1, NAV_SHUFFLE, 0xe73c);
  mdisplay_hlvf_DrawString((ST7735_LCD_PIXEL_WIDTH >> 1), 3, "11:30", COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);

  // Track Name, album, artist information
  mdisplay_hlvf_DrawColorWheelString((ST7735_LCD_PIXEL_WIDTH >> 1), 45, tptr->trackName, 0, 255, 153, 77, FONT_8X12, ALIGNMENT_CENTER);

  char sBuf[100] = "";
  sprintf(sBuf,"%s - %s", tptr->artistName, tptr->albumName);
  mdisplay_hlvf_DrawString((ST7735_LCD_PIXEL_WIDTH >> 1), 60, sBuf, COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);

  // Progress bar
  mdisplay_hlvf_FillRectangle(10, 80, ST7735_LCD_PIXEL_WIDTH - 50, 4, 0xe73c);
  mdisplay_hlvf_FillRectangle(10, 80, ST7735_LCD_PIXEL_WIDTH - 80, 4, COLOR_GRAY);
  mdisplay_hlvf_DrawString(125, 78, "3:17", COLOR_GRAY, FONT_5X7, ALIGNMENT_LEFT);

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

  // Wipe screen
  mdisplay_hlvf_FillScreen(COLOR_WHITE);
}
