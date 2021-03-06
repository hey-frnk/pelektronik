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
#include "MenuDisplay.h"

#ifndef DEBUG
#include "st7735.h"
#endif

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"

#define TOPBAROFFSET          18
#define DISTANCEMULTIPLIER    22

struct MP3Display_vTable MenuDisplay_vS = {&MenuDisplay_getType, &MenuDisplay_show};

uint8_t MenuDisplay_getType(void *iptr) {
  #ifdef DEBUG
  printf("MenuDisplay: Type: %hhu\n", ((MenuDisplay *) iptr)->super.type);
  #endif
  return ((MenuDisplay *) iptr)->super.type;
}

void MenuDisplay_show(void *iptr) {
  MenuDisplay *tptr = ((MenuDisplay *) iptr);
  #ifdef DEBUG
  printf("MenuDisplay: Show Method of MenuDisplay Called!\n");
  #endif

  // Draw horizontal menu element lines
  uint32_t _itemPos = tptr->itemPos;
  uint32_t _iStart = (_itemPos < 5) ? 0 : tptr->itemPos - 4;
  uint32_t _iMax = (tptr->itemArraySize > 4) ? 5 : tptr->itemArraySize;

  uint8_t enableScrollBar = tptr->itemArraySize > 5;
  if(enableScrollBar){
    // Draw scroll bar base
    mdisplay_hlvf_DrawRectangle(_global_width - 5, TOPBAROFFSET + 1, 5, _global_height - TOPBAROFFSET - 1, COLOR_GRAY);
    // Fill scroll bar position
    uint8_t _gbLength = _global_height - TOPBAROFFSET - 2;
    uint8_t _sbLength = (5.0f / tptr->itemArraySize) * (float)(_gbLength);
    uint8_t _sbOffset = ((_gbLength - _sbLength) * _itemPos) / (float)(tptr->itemArraySize - 1);
    mdisplay_hlvf_FillRectangle(_global_width - 4, TOPBAROFFSET + 1 + _sbOffset, 3, _sbLength, COLOR_GRAY);
  }

  // Draw menu elements
  for(uint32_t i = _iStart; i < _iMax + _iStart; ++i){
    uint32_t _iStartOffset = (i - _iStart) * DISTANCEMULTIPLIER;
    if(40 + _iStartOffset < _global_height){
      #ifdef DEBUG
          mdisplay_hlvf_DrawRectangle(0, 40 + _iStartOffset, enableScrollBar ? _global_width - 5 : _global_width, 1, 0xD6BA);
      #else
          st7735_DrawHLine(0xD6BA, 0, 40 + _iStartOffset, enableScrollBar ? _global_width - 5 : _global_width);
      #endif
    }

    char buf[15];
    strncpy(buf, tptr->itemArray[i], 14);
    buf[14] = 0;
    if(i == tptr->itemPos){ // Focus!
      mdisplay_hlvf_DrawColorWheelStringFast(25, TOPBAROFFSET + 5 + _iStartOffset, buf, FONT_8X12, ALIGNMENT_LEFT);
      // Draw advanced color icon?
      if(tptr->itemColorArray[i] == COLOR_WHITE) mdisplay_hlvf_DrawColorIcon(3, TOPBAROFFSET + 3 + _iStartOffset, tptr->itemIconArray[i], 0);
      else mdisplay_hlvf_DrawIcon(3, TOPBAROFFSET + 3 + _iStartOffset, tptr->itemIconArray[i], tptr->itemColorArray[i]);

      mdisplay_hlvf_DrawChar(140, TOPBAROFFSET + 5 + _iStartOffset, '>', COLOR_GRAY, FONT_8X12);
    }
    else{ // Defocus!
      mdisplay_hlvf_DrawString(25, TOPBAROFFSET + 5 + _iStartOffset, buf, COLOR_GRAY, FONT_8X12, ALIGNMENT_LEFT);

      if(tptr->itemColorArray[i] == COLOR_WHITE) mdisplay_hlvf_DrawColorIcon(3, TOPBAROFFSET + 3 + _iStartOffset, tptr->itemIconArray[i], 1);
      else mdisplay_hlvf_DrawIcon(3, TOPBAROFFSET + 3 + _iStartOffset, tptr->itemIconArray[i], COLOR_GRAY);
    }
  }


}

void MenuDisplay_updateItems(MenuDisplay *iptr, char **itemArray, uint8_t *itemIconArray, uint16_t *itemColorArray, uint32_t itemArraySize){
  iptr->itemArray = itemArray;
  iptr->itemIconArray = itemIconArray;
  iptr->itemColorArray = itemColorArray;
  iptr->itemArraySize = itemArraySize;
}

void MenuDisplay_itemUp(MenuDisplay *iptr){
  if(iptr->itemPos > 0) --(iptr->itemPos);
  mdisplay_hlvf_FillScreen(COLOR_WHITE);  // CLR
  // iptr->super.show((MP3Display *)iptr);
}

void MenuDisplay_itemDown(MenuDisplay *iptr){
  if(iptr->itemPos < iptr->itemArraySize - 1) ++(iptr->itemPos);
  mdisplay_hlvf_FillScreen(COLOR_WHITE);  // CLR
  // iptr->super.show((MP3Display *)iptr);
}

void MenuDisplay_deInit(MenuDisplay *iptr) {
  free(iptr->itemIconArray);
  free(iptr->itemColorArray);
  free(iptr->itemArray);
}

void MenuDisplay_init(MenuDisplay *iptr) {
  #ifdef DEBUG
  printf("MenuDisplay initialized\n");
  #endif
  MP3Display_init(&iptr->super);
  iptr->super.vt = &MenuDisplay_vS;
  iptr->super.type = MDISPLAY_TYPE_MENU;
  iptr->updateItems = MenuDisplay_updateItems;
  iptr->itemUp = MenuDisplay_itemUp;
  iptr->itemDown = MenuDisplay_itemDown;
  iptr->deInit = MenuDisplay_deInit;

  iptr->itemPos = 0;

  // Wipe screen
  mdisplay_hlvf_FillScreen(COLOR_WHITE);
}
