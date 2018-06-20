#include <stdio.h>
#include "MP3Display.h"

#ifndef DEBUG
#include "st7735.h"
#endif

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"
#include "RTCI.h"

static uint8_t MP3Display_getType(void *iptr) {
  return ((MP3Display *)iptr)->vt->getType(iptr);
}

static void MP3Display_show(void *iptr) {
  // Universal header
  // UI Line 0xE73C
  #ifdef DEBUG
    mdisplay_hlvf_DrawRectangle(0, 18, _global_width, 1, COLOR_GRAY);
  #else
    st7735_DrawHLine(COLOR_GRAY, 0, 18, _global_width);
  #endif

  ((MP3Display *)iptr)->vt->show(iptr);
}

static void setBatteryState(MP3Display *iptr, uint8_t batteryLevel) {
  iptr->batteryLevel = batteryLevel;
  printf("Battery Level Set To: %hhu\n", iptr->batteryLevel);

  // Battery Display
  mdisplay_hlvf_DrawRectangle(136, 4, 18, 10, COLOR_GRAY);
  mdisplay_hlvf_DrawRectangle(154, 6, 2, 6, COLOR_GRAY);

  for(int i = 0; i < 4; ++i){
    // Write green bar or overwrite with white
    if(i < batteryLevel) mdisplay_hlvf_FillRectangle(137 + (i << 2) , 6, 3, 6, (batteryLevel < 2) ? 0x61C4 : 0x23C7);
    else mdisplay_hlvf_FillRectangle(137 + (i << 2) , 6, 3, 6, COLOR_WHITE);
  }
}

static void updateTime(MP3Display *iptr) {
  mtime t;
  RTCI_getTime(&t);

  char tConst[9] = {  (t.hr / 10) + '0',
		  	  	  	  (t.hr % 10) + '0',
					  ':',
					  (t.min / 10) + '0',
					  (t.min % 10) + '0',
					  ':',
					  (t.sec / 10) + '0',
					  (t.sec % 10) + '0',
					  0};

  mdisplay_hlvf_FillRectangle(50, 4, 62, 9, COLOR_WHITE);
  mdisplay_hlvf_DrawString((_global_width >> 1), 3, tConst, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
}

void MP3Display_init(MP3Display *iptr) {
  printf("MP3Display initialized\n");

  iptr->init = MP3Display_init;
  iptr->setBatteryState = setBatteryState;
  iptr->updateTime = updateTime;
  iptr->getType = MP3Display_getType;
  iptr->show = MP3Display_show;
}
