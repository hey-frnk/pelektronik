/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "RTCI.h"

#ifndef MP3DISPLAY_MESSAGE_HPP
#define MP3DISPLAY_MESSAGE_HPP

#define _MESSAGEPROMPT_BRIGHTNESSSETLEVELNUM 5
#define _msgPStart 22

extern int8_t currentBrightness;
extern int8_t previousBrightness;

extern mtime tSet;
extern mtime tNew;

// Message Box Class
typedef enum MESSAGE_PROMPT_POSITION {
  MESSAGE_PROMPT_LEFTFOCUSED    = 0,
  MESSAGE_PROMPT_RIGHTFOCUSED   = 1,
  MESSAGE_PROMPT_SELECTION1     = 2,
  MESSAGE_PROMPT_SELECTION2     = 3
} MESSAGE_PROMPT_POSITION;

typedef enum MESSAGE_PROMPT_RESULT {
  MESSAGE_PROMPT_RESULT_OK,
  MESSAGE_PROMPT_RESULT_CANCEL
} MESSAGE_PROMPT_RESULT;

typedef struct MessagePrompt {
  void        (*show)                   (struct MessagePrompt *iptr);
  void        (*moveLeft)               (struct MessagePrompt *iptr);
  void        (*moveRight)              (struct MessagePrompt *iptr);
  MESSAGE_PROMPT_POSITION (*getFocus)(struct MessagePrompt *iptr);

  char      *btnLeft, *btnRight, *msg;
  uint8_t   position, maxFocusElements;
} MessagePrompt;

void MessagePrompt_init(
  MessagePrompt                     *iptr,              // this pointer
  MESSAGE_PROMPT_POSITION           initFocus,          // which of the one or two buttons should initially be focused
  char                              *message,           // Message
  char                              *btnLeft,           // Text of left button
  char                              *btnRight,          // Test of right button
  uint8_t                           maxFocusElements
);

/* msgPromptHandler:
 * (init): create a message instance internally
 * msgPromptHandler[0]: can be used for message specific static drawing init
 * (show): button dynamic drawing init
 * WHILE(1)
 *    msgPromptHandler[1]: dynamic init, run every loop
 *    (check for button input)
 *    (case button left): msgPromptHandler[2]: use this to redraw message specific dynamic draws
 *    (case button right): msgPromptHandler[3]: use this to redraw message specific dynamic draws
 *    (case button menu):
 *        msgPromptHandler[4]: use this to redraw message specific dynamic draws
 *        (focus on OK or cancel button) ? break, return OK or cancel : continue
 *    (case button back):
 *        msgPromptHandler[5]: use this to redraw message specific dynamic draws
 *        (focus on OK or cancel button) ? break, return cancel : continue
 * END WHILE
*/

MESSAGE_PROMPT_RESULT MsgBox(
  void *iptr,
  void (*msgPromptHandler[6])(MessagePrompt *, void *), // Array of msgPromptHandlers. 0: routine, 1: left, 2: menu, 3: back, 4: right procedures
  MESSAGE_PROMPT_POSITION initFocus,                    // Just like init. So no manual initialization
  char *message, char *btnLeft, char *btnRight,
  uint8_t maxFocusElements             // Maximum of focusable elements
);

// Message Handlers
void MessagePrompt_DrawHandle_DrawMessage(MessagePrompt *, void *);

// Brightness Set Handlers
void MessagePrompt_Brightness_DrawUpdate(MessagePrompt *, void *);
void MessagePrompt_Brightness_DrawInit(MessagePrompt *, void *);
void MessagePrompt_Brightness_Menu(MessagePrompt *, void *);
void MessagePrompt_Brightness_Back(MessagePrompt *, void *);

// Time Set Handlers
void MessagePrompt_TimeSet_Init(MessagePrompt *, void *);
void MessagePrompt_TimeSet_DrawUpdate(MessagePrompt *, void *);
void MessagePrompt_TimeSet_Menu(MessagePrompt *, void *);
void MessagePrompt_TimeSet_Back(MessagePrompt *, void *);

#endif

#ifdef __cplusplus
}
#endif
