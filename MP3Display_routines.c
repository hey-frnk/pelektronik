#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MP3Display.h"
#include "MP3DI.h"
#include "TrackDisplay.h"
#include "TrackEQDisplay.h"
#include "MP3Display_routines.h"

// Active Instance
MP3Display* INSTANCE_Active = NULL;

// Possible allocated instances
TrackDisplay* INSTANCE_TrackDISPLAY = NULL;
TrackEQDisplay* INSTANCE_TrackEQDISPLAY = NULL;

// Current state variable
MP3Display_State mp3display_state = MP3DISPLAYSTATE_BOOT;

void (* MP3DisplayState_Routine[])(void) = { _routine_BOOT,        // Corresponding routines
                                              _routine_PLAY, _routine_TRACKLIST,
                                              _routine_MAINMENU, _routine_SETTINGS,
                                              _routine_RECORD, _routine_SLEEP,
                                              _routine_SHUTDOWN };

Track* currentTrack = NULL;

void _routine_BOOT(void){
  currentTrack = malloc(sizeof(Track));
  static char trackName[] = "Technicolour Beat";
  static char artistName[] = "Oh Wonder";
  static char albumName[] = "Oh Wonder";
  currentTrack->trackName = trackName;
  currentTrack->artistName = artistName;
  currentTrack->albumName = albumName;
  currentTrack->length = 179;

  printf("Boot Successful. Entering next state (PLAY)\n");
  MP3Display_State nextState = MP3DISPLAYSTATE_PLAY;
  (*MP3DisplayState_Routine[nextState])();
  mp3display_state = nextState;
}

void _routine_PLAY(void){
  // Check for Instance availability
  if((INSTANCE_TrackDISPLAY == NULL) && (INSTANCE_TrackEQDISPLAY == NULL)){
    // Instance being called for the first time, create standard track display instance
    INSTANCE_TrackDISPLAY = malloc(sizeof(TrackDisplay));
    TrackDisplay_init(INSTANCE_TrackDISPLAY);

    // Set active instance
    INSTANCE_Active = (MP3Display *)INSTANCE_TrackDISPLAY;

    // Set track information
    if(currentTrack == NULL) return;  // Oh something went wrong
    INSTANCE_TrackDISPLAY->setTrackInfo(INSTANCE_TrackDISPLAY, currentTrack->trackName, currentTrack->artistName, currentTrack->albumName, currentTrack->length);
    INSTANCE_TrackDISPLAY->super.show(INSTANCE_TrackDISPLAY);
  }

  // Display is active
  if(INSTANCE_TrackDISPLAY != NULL){
    printf("Track Display On.");
    INSTANCE_TrackDISPLAY->super.show(INSTANCE_TrackDISPLAY);
  } else if(INSTANCE_TrackEQDISPLAY != NULL){

  }

}

void _routine_TRACKLIST(void){
  printf("Hello3\n");
}

void _routine_MAINMENU(void){
  printf("Hello4\n");
}

void _routine_SETTINGS(void){
  printf("Hello5\n");
}

void _routine_RECORD(void){
  printf("Hello6\n");
}

void _routine_SLEEP(void){
  printf("Hello7\n");
}

void _routine_SHUTDOWN(void){
  printf("Hello8\n");
}
