/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 *
 * TABLE OF CONTENTS. Just type in this to navigate
 * - MESSAGE_CLASS_INTERNAL
 * - MESSAGE_CLASS_METHODS
 * - MESSAGE_HANDLERS
 * - BRIGHTNESS_SET_HANDLERS
 * - CLOCK_SET_HANDLERS
 * - MESSAGE_EXAMPLE_USES
 */

#include <stdio.h>
#include <string.h>
#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"
#include "MP3Display.h"
#include "MP3Display_Message.h"
#include "MP3BI.h"

#ifndef DEBUG
#include "st7735.h"
#endif

/* ++++++++++++ @SEARCH: MESSAGE_CLASS_INTERNAL ++++++++++++*/
/**
  * @brief  Internal button draw function
  * @input  x, y coordinates, button size, button message, focus flag
  * @retval None
  */
static void _MessagePrompt_DrawButton(uint8_t x, uint8_t y, uint8_t width, char *msg, uint8_t focused) {
  mdisplay_hlvf_DrawRectangle(x - (width >> 1), y - 10, width, 20, COLOR_GRAY);
  if(focused) mdisplay_hlvf_DrawColorWheelString(x - 1, y - 6, msg, 145, 152, 180, 76, FONT_8X12, ALIGNMENT_CENTER);
  else mdisplay_hlvf_DrawString(x - 1, y - 6, msg, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
}

/* ++++++++++++ @SEARCH: MESSAGE_CLASS_METHODS ++++++++++++*/
/**
  * @brief  Dynamic show function that draws the buttons
  * @input  Current message prompt instance
  * @retval None
  */
void MessagePrompt_show(MessagePrompt *iptr) { // Dynamic elements
  #ifdef DEBUG
  printf("MsgPrompt Show Method called\n");
  #endif

  // Draw Buttons
  if(iptr->btnRight == NULL) {  // Oh just one button
    _MessagePrompt_DrawButton(_global_width >> 1, _msgPStart + 65, 80, iptr->btnLeft, 1);
  } else {
    _MessagePrompt_DrawButton((_global_width >> 1) - 33, _msgPStart + 65, 60, iptr->btnLeft, iptr->position == MESSAGE_PROMPT_LEFTFOCUSED);
    _MessagePrompt_DrawButton((_global_width >> 1) + 33, _msgPStart + 65, 60, iptr->btnRight, iptr->position == MESSAGE_PROMPT_RIGHTFOCUSED);
  }
}

/**
  * @brief  Focus the previous prompt item
  * @input  Current message prompt instance
  * @retval None
  */
void MessagePrompt_moveLeft(MessagePrompt *iptr) {
  if(iptr->position == 0) {
    iptr->position = iptr->maxFocusElements - 1;
    return;
  }
  --(iptr->position);
}

/**
  * @brief  Focus the next prompt item
  * @input  Current message prompt instance
  * @retval None
  */
void MessagePrompt_moveRight(MessagePrompt *iptr) {
  ++(iptr->position);
  if(iptr->position == iptr->maxFocusElements) (iptr->position) = 0;
}

/**
  * @brief  Get current focus
  * @input  Current message prompt instance
  * @retval Focus index
  */
MESSAGE_PROMPT_POSITION MessagePrompt_getFocus(MessagePrompt *iptr) {
  return iptr->position;
}

/**
  * @brief  Constructor of message prompt. Initialize with all parameters
  * @input  Current messag prompt instance (this pointer), initial focused item, message, left/right button text
  * @retval None
  */
void MessagePrompt_init(MessagePrompt *iptr, MESSAGE_PROMPT_POSITION initFocus, char *message, char *btnLeft, char *btnRight, uint8_t maxFocusElements) {
  printf("Message prompt initialized\n");
  iptr->msg = message;
  iptr->btnLeft = btnLeft;
  iptr->btnRight = btnRight;

  iptr->show = MessagePrompt_show;
  iptr->moveLeft = MessagePrompt_moveLeft;
  iptr->moveRight = MessagePrompt_moveRight;
  iptr->getFocus = MessagePrompt_getFocus;
  iptr->position = initFocus;
  iptr->maxFocusElements = maxFocusElements;
}

/**
  * @brief  VB Style Message Box
  * @input
  * @retval RESULT (OK or Cancel)
  */
MESSAGE_PROMPT_RESULT MsgBox(void *iptr, void (*msgPromptHandler[6])(MessagePrompt *, void *), MESSAGE_PROMPT_POSITION initFocus, char *message, char *btnLeft, char *btnRight, uint8_t maxFocusElements) {
  MessagePrompt p;
  MP3Display *tptr = (MP3Display *)iptr;
  MessagePrompt_init(&p, initFocus, message, btnLeft, btnRight, maxFocusElements);
  uint8_t bLeft = 0, bMenu = 0, bBack = 0, bRight = 0;

  mdisplay_hlvf_FillRectangle(10, _msgPStart, 140, 90, 0xe73c);
  mdisplay_hlvf_DrawRectangle(10, _msgPStart, 140, 90, 0x0452);
  mdisplay_hlvf_DrawRectangle(11, _msgPStart + 1, 138, 88, 0x0452);

  if(msgPromptHandler[0]) (*msgPromptHandler[0])(&p, iptr);  // Draw Handler
  p.show(&p);

  while(1) {
    #ifdef DEBUG
    printf("Message Box Trap Loop\n");
    #endif

    if(msgPromptHandler[1]) (*msgPromptHandler[1])(&p, iptr);
    MP3BI_retrieveAllButtonStates(&bLeft, &bMenu, &bBack, &bRight);

    if(bLeft == BUTTON_STATE_SHORTPRESS) {
      p.moveLeft(&p);
      p.show(&p);
      if(msgPromptHandler[2]) (*msgPromptHandler[2])(&p, iptr);
      continue;
    }
    else if(bRight == BUTTON_STATE_SHORTPRESS) {
      p.moveRight(&p);
      p.show(&p);
      if(msgPromptHandler[3]) (*msgPromptHandler[3])(&p, iptr);
      continue;
    }
    else if(bMenu == BUTTON_STATE_SHORTPRESS) {
      if(msgPromptHandler[4]) (*msgPromptHandler[4])(&p, iptr);
      if(p.position == MESSAGE_PROMPT_LEFTFOCUSED) {
        tptr->clear(tptr);
        return MESSAGE_PROMPT_RESULT_OK;
      }
      else if(p.position == MESSAGE_PROMPT_RIGHTFOCUSED) {
        tptr->clear(tptr);
        return MESSAGE_PROMPT_RESULT_CANCEL;
      }
      else continue;
    }
    else if(bBack == BUTTON_STATE_SHORTPRESS) {
      if(msgPromptHandler[5]) (*msgPromptHandler[5])(&p, iptr);
      if(p.position == MESSAGE_PROMPT_LEFTFOCUSED || p.position == MESSAGE_PROMPT_RIGHTFOCUSED) {
        tptr->clear(tptr);
        return MESSAGE_PROMPT_RESULT_CANCEL;
      }
      else continue;
    }
  }
}




/* ++++++++++++ @SEARCH: MESSAGE_HANDLERS ++++++++++++*/
/**
  * @brief  Default message draw handle
  * @input  Current message prompt instance
  * @retval None
  */
void MessagePrompt_DrawHandle_DrawMessage(MessagePrompt *mptr, void *iptr) {
  // Draw Error Message.
  mdisplay_hlvf_DrawTwoLineString(_global_width >> 1, _msgPStart + 18, \
                                  mptr->msg, \
                                  COLOR_GRAY, FONT_8X12, \
                                  ALIGNMENT_CENTER, \
                                  16);
}


/* ++++++++++++ @SEARCH: BRIGHTNESS_SET_HANDLERS ++++++++++++*/
/**
  * @brief  Default brihgtness set draw/logic handle
  * @input  Current message prompt instance
  * @retval None
  */
const char* _MessagePrompt_BrightnessSetLevels[_MESSAGEPROMPT_BRIGHTNESSSETLEVELNUM] = {"Fucking Dark", "Dark", "Pleasant", "Bright", "Awfully Bright"};
int8_t currentBrightness = 4;
int8_t previousBrightness;

void MessagePrompt_Brightness_Init() {
  previousBrightness = currentBrightness;
}

void MessagePrompt_Brightness_DrawUpdate(MessagePrompt *mptr, void *iptr) {
  mdisplay_hlvf_FillRectangle((_global_width >> 1) - 60, _msgPStart + 15, 120, 32, 0xe73c);

  // Draw Brightness string (focus, up, down)
  mdisplay_hlvf_DrawRectangle((_global_width >> 1) - 60, _msgPStart + 24, 120, 14, COLOR_LGRAY);
  if(mptr->position == MESSAGE_PROMPT_SELECTION1) mdisplay_hlvf_DrawColorWheelStringFast(_global_width >> 1, _msgPStart + 25, _MessagePrompt_BrightnessSetLevels[currentBrightness], FONT_8X12, ALIGNMENT_CENTER);
  else mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, _MessagePrompt_BrightnessSetLevels[currentBrightness], COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
  if(currentBrightness - 1 >= 0) mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 15, _MessagePrompt_BrightnessSetLevels[currentBrightness - 1], COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
  if(currentBrightness + 1 < _MESSAGEPROMPT_BRIGHTNESSSETLEVELNUM) mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 40, _MessagePrompt_BrightnessSetLevels[currentBrightness + 1], COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
}

void MessagePrompt_Brightness_Menu(MessagePrompt *mptr, void *iptr) {
  if(mptr->position == MESSAGE_PROMPT_SELECTION1) {
    if(currentBrightness > 0) --currentBrightness;
    MessagePrompt_Brightness_DrawUpdate(mptr, iptr);
    mdisplay_hlvf_SetBrightness(currentBrightness);
  }
}

void MessagePrompt_Brightness_Back(MessagePrompt *mptr, void *iptr) {
  if(mptr->position == MESSAGE_PROMPT_SELECTION1) {
    if(currentBrightness < _MESSAGEPROMPT_BRIGHTNESSSETLEVELNUM - 1) ++currentBrightness;
    MessagePrompt_Brightness_DrawUpdate(mptr, iptr);
    mdisplay_hlvf_SetBrightness(currentBrightness);
  }
}


/* ++++++++++++ @SEARCH: CLOCK_SET_HANDLERS ++++++++++++*/
/**
  * @brief  Default brihgtness set draw/logic handle
  * @input  Current message prompt instance
  * @retval None
  */
mtime tSet, tNew;

void MessagePrompt_TimeSet_Init(MessagePrompt *mptr, void *iptr) {
  // mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 3, "HAVE A COLORFUL TIME.", COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
  RTCI_getTime(&tSet);
  tNew = tSet;
  MessagePrompt_TimeSet_DrawUpdate(mptr, iptr);
}

void MessagePrompt_TimeSet_DrawUpdate(MessagePrompt *mptr, void *iptr) {
  mdisplay_hlvf_FillRectangle((_global_width >> 1) - 60, _msgPStart + 15, 120, 32, 0xe73c);

  // Super inefficient!
  char tConstSep[6] = {' ', ' ', ':', ' ', ' ', 0};
  char tConstHours[6] = {
            (tNew.hr / 10) + '0',
		  	  	(tNew.hr % 10) + '0',
					  ' ', // ':'
					  ' ', // (tSet.min / 10) + '0',
					  ' ', // (tSet.min % 10) + '0',
					  0};
  char tConstMinutes[6] = {
            ' ', // (tSet.hr / 10) + '0',
		  	  	' ', // (tSet.hr % 10) + '0',
					  ' ', // ':'
					  (tNew.min / 10) + '0',
					  (tNew.min % 10) + '0',
					  0};

  char tConstPrevHours[3] = {
            ((tNew.hr > 0) ? tNew.hr - 1 : 23) / 10 + '0',
            ((tNew.hr > 0) ? tNew.hr - 1 : 23) % 10 + '0',
            0};
  char tConstNextHours[3] = {
            ((tNew.hr + 1) % 24) / 10 + '0',
            ((tNew.hr + 1) % 24) % 10 + '0',
            0};
  char tConstPrevMinutes[3] = {
            ((tNew.min > 0) ? tNew.min - 1 : 59) / 10 + '0',
            ((tNew.min > 0) ? tNew.min - 1 : 59) % 10 + '0',
            0};
  char tConstNextMinutes[3] = {
            ((tNew.min + 1) % 60) / 10 + '0',
            ((tNew.min + 1) % 60) % 10 + '0',
            0};

  mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstSep, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);

  // Prev/Next hours
  mdisplay_hlvf_DrawString((_global_width >> 1) - 10, _msgPStart + 15, tConstPrevHours, COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
  mdisplay_hlvf_DrawString((_global_width >> 1) - 10, _msgPStart + 40, tConstNextHours, COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
  mdisplay_hlvf_DrawString((_global_width >> 1) + 10, _msgPStart + 15, tConstPrevMinutes, COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);
  mdisplay_hlvf_DrawString((_global_width >> 1) + 10, _msgPStart + 40, tConstNextMinutes, COLOR_GRAY, FONT_5X7, ALIGNMENT_CENTER);

  // Draw hours colorfully, seconds BW
  mdisplay_hlvf_DrawRectangle((_global_width >> 1) - 26, _msgPStart + 24, 56, 14, COLOR_LGRAY);
  if(mptr->position == MESSAGE_PROMPT_SELECTION1) {
    mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstHours, COLOR_DARKBLUE, FONT_8X12, ALIGNMENT_CENTER);
    mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstMinutes, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
  } else if(mptr->position == MESSAGE_PROMPT_SELECTION2) {
    mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstHours, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
    mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstMinutes, COLOR_DARKBLUE, FONT_8X12, ALIGNMENT_CENTER);
  } else {
    mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstMinutes, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
    mdisplay_hlvf_DrawString(_global_width >> 1, _msgPStart + 25, tConstHours, COLOR_GRAY, FONT_8X12, ALIGNMENT_CENTER);
  }
}

void MessagePrompt_TimeSet_Menu(MessagePrompt *mptr, void *iptr) {
  if(mptr->position == MESSAGE_PROMPT_SELECTION1) { // hr set back
    if(tNew.hr > 0) --tNew.hr;
    else if(tNew.hr == 0) tNew.hr = 23;
  } else if(mptr->position == MESSAGE_PROMPT_SELECTION2) {
    if(tNew.min > 0) --tNew.min;
    else if(tNew.min == 0) tNew.min = 59;
  } else return;
  RTCI_setTime(&tNew);
  MessagePrompt_TimeSet_DrawUpdate(mptr, iptr);
}

void MessagePrompt_TimeSet_Back(MessagePrompt *mptr, void *iptr) {
  if(mptr->position == MESSAGE_PROMPT_SELECTION1) tNew.hr = (tNew.hr + 1) % 24;
  else if(mptr->position == MESSAGE_PROMPT_SELECTION2) tNew.min = (tNew.min + 1) % 60;
  else return;
  RTCI_setTime(&tNew);
  MessagePrompt_TimeSet_DrawUpdate(mptr, iptr);
}



/* ++++++++++++ @SEARCH: MESSAGE_EXAMPLE_USES ++++++++++++*/
/*void (* TestMsgPromptHandler[6])(MessagePrompt *, void *) = {MessagePrompt_DrawHandle_DrawMessage, TestPromptInitHandler, NULL, NULL, TestPromptMenuHandler, TestPromptMenuHandler};
MESSAGE_PROMPT_RESULT pres = MsgBox((void *)INSTANCE_TrackDISPLAY, TestMsgPromptHandler, MESSAGE_PROMPT_LEFTFOCUSED, "Error playing: Type Unsupported", "OK", "Cancel", 2);
if(pres == MESSAGE_PROMPT_RESULT_OK) {
  printf("OK clicked!\n");
} else {
  printf("Cancel clicked!\n");
}*/

/*void (* BrightnessPromptHandler[6])(MessagePrompt *, void *) = {MessagePrompt_Brightness_DrawUpdate, TestPromptInitHandler, MessagePrompt_Brightness_DrawUpdate, MessagePrompt_Brightness_DrawUpdate, MessagePrompt_Brightness_Menu, MessagePrompt_Brightness_Back};
MESSAGE_PROMPT_RESULT pres = MsgBox((void *)INSTANCE_TrackDISPLAY, BrightnessPromptHandler, MESSAGE_PROMPT_SELECTION1, NULL, "Set", "Cancel", 3);
if(pres == MESSAGE_PROMPT_RESULT_OK) {
  printf("Brightness set!\n");
} else {
  printf("Set canceled!\n");
}*/

/*void (*TimeSetPromptHandler[6])(MessagePrompt *, void *) = {MessagePrompt_TimeSet_Init, TestPromptInitHandler, MessagePrompt_TimeSet_DrawUpdate, MessagePrompt_TimeSet_DrawUpdate, MessagePrompt_TimeSet_Menu, MessagePrompt_TimeSet_Back};
MESSAGE_PROMPT_RESULT pres = MsgBox((void *)INSTANCE_TrackDISPLAY, TimeSetPromptHandler, MESSAGE_PROMPT_SELECTION1, NULL, "Set", "Cancel", 4);
if(pres == MESSAGE_PROMPT_RESULT_OK) {
  printf("Time set!\n");
} else {
  printf("Set canceled!\n");
}*/



// Hello World
