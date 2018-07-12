/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MP3DISPLAY_ROUTINES_H
#define MP3DISPLAY_ROUTINES_H

#include "MP3Display.h"
#include "TrackDisplay.h"

typedef enum MP3Display_State {
  MP3DISPLAYSTATE_BOOT        = 0, // Boot state: boot information display
  MP3DISPLAYSTATE_PLAY        = 1, // Play state: Display track information with or without EQ
  MP3DISPLAYSTATE_TRACKLIST   = 2, // Menu track list state: show menu track list
  MP3DISPLAYSTATE_MAINMENU    = 3, // Main menu state: show main menu
  MP3DISPLAYSTATE_SETTINGS    = 4, // Settings menu state: show settings menu
  MP3DISPLAYSTATE_PICTURE     = 5, // Picture display state
  MP3DISPLAYSTATE_RECORD      = 6, // Voice recording state
  MP3DISPLAYSTATE_SLEEP       = 7, // Display off
  MP3DISPLAYSTATE_SHUTDOWN    = 8  // Shut down state
} MP3Display_State;

void _routine_BOOT(void);
void _routine_PLAY(void);
void _routine_TRACKLIST(void);
void _routine_MAINMENU(void);
void _routine_SETTINGS(void);
void _routine_PICTURE(void);
void _routine_RECORD(void);
void _routine_SLEEP(void);
void _routine_SHUTDOWN(void);

// Active Instance
extern MP3Display* INSTANCE_Active;

// Possible allocated instances
extern TrackDisplay* INSTANCE_TrackDISPLAY;

// Current state variable
extern MP3Display_State mp3display_state;

extern void (* MP3DisplayState_Routine[])(void);

#endif

#ifdef __cplusplus
}
#endif
