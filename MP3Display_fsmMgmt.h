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
