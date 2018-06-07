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
  printf("MenuDisplay: Type: %hhu\n", ((MenuDisplay *) iptr)->super.type);
  return ((MenuDisplay *) iptr)->super.type;
}

void MenuDisplay_show(void *iptr) {
  MenuDisplay *tptr = ((MenuDisplay *) iptr);
  printf("MenuDisplay: Show Method of MenuDisplay Called!\n");

  // Draw horizontal menu element lines
  for(uint8_t i = 0; i < tptr->itemArraySize; ++i){
    #ifdef DEBUG
        mdisplay_hlvf_DrawRectangle(0, 40 + DISTANCEMULTIPLIER * i, _global_width, 1, 0xD6BA);
    #else
        st7735_DrawHLine(0xD6BA, 0, 40 + DISTANCEMULTIPLIER * i, _global_width);
    #endif

    mdisplay_hlvf_DrawIcon(3, TOPBAROFFSET + 3 + DISTANCEMULTIPLIER * i, tptr->itemIconArray[i], COLOR_GRAY);
    if(i == tptr->itemPos) mdisplay_hlvf_DrawColorWheelStringFast(25, TOPBAROFFSET + 5 + DISTANCEMULTIPLIER * i, tptr->itemArray[i], FONT_8X12, ALIGNMENT_LEFT);
    else mdisplay_hlvf_DrawString(25, TOPBAROFFSET + 5 + DISTANCEMULTIPLIER * i, tptr->itemArray[i], COLOR_GRAY, FONT_8X12, ALIGNMENT_LEFT);
  }


}

void MenuDisplay_updateItems(MenuDisplay *iptr, char **itemArray, uint8_t *itemIconArray, uint32_t itemArraySize){
  iptr->itemArray = itemArray;
  iptr->itemIconArray = itemIconArray;
  iptr->itemArraySize = itemArraySize;
}

void MenuDisplay_itemUp(MenuDisplay *iptr){
  if(iptr->itemPos > 0) --(iptr->itemPos);
  iptr->super.show((MP3Display *)iptr);
}

void MenuDisplay_itemDown(MenuDisplay *iptr){
  if(iptr->itemPos < iptr->itemArraySize - 1) ++(iptr->itemPos);
  iptr->super.show((MP3Display *)iptr);
}



void MenuDisplay_init(MenuDisplay *iptr) {
  printf("MenuDisplay initialized\n");
  MP3Display_init(&iptr->super);
  iptr->super.vt = &MenuDisplay_vS;
  iptr->super.type = MDISPLAY_TYPE_MENU;
  iptr->updateItems = MenuDisplay_updateItems;
  iptr->itemUp = MenuDisplay_itemUp;
  iptr->itemDown = MenuDisplay_itemDown;

  iptr->itemPos = 0;

  // Wipe screen
  mdisplay_hlvf_FillScreen(COLOR_WHITE);
}
