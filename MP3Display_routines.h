#ifndef MP3DISPLAY_ROUTINES_H
#define MP3DISPLAY_ROUTINES_H

typedef enum MP3Display_State {
  MP3DISPLAYSTATE_BOOT        = 0, // Boot state: boot information display
  MP3DISPLAYSTATE_PLAY        = 1, // Play state: Display track information with or without EQ
  MP3DISPLAYSTATE_TRACKLIST   = 2, // Menu track list state: show menu track list
  MP3DISPLAYSTATE_MAINMENU    = 3, // Main menu state: show main menu
  MP3DISPLAYSTATE_SETTINGS    = 4, // Settings menu state: show settings menu
  MP3DISPLAYSTATE_RECORD      = 5, // Voice recording state
  MP3DISPLAYSTATE_SLEEP       = 6, // Display off
  MP3DISPLAYSTATE_SHUTDOWN    = 7  // Shut down state
} MP3Display_State;

void _routine_BOOT(void);
void _routine_PLAY(void);
void _routine_TRACKLIST(void);
void _routine_MAINMENU(void);
void _routine_SETTINGS(void);
void _routine_RECORD(void);
void _routine_SLEEP(void);
void _routine_SHUTDOWN(void);

// Active Instance
extern MP3Display* INSTANCE_Active;

// Possible allocated instances
extern TrackDisplay* INSTANCE_TrackDISPLAY;
extern TrackEQDisplay* INSTANCE_TrackEQDISPLAY;

// Current state variable
extern MP3Display_State mp3display_state;

extern void (* MP3DisplayState_Routine[])(void);

#endif
