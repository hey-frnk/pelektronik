#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MP3Display.hpp"
#include "TrackDisplay.hpp"
#include "TrackEQDisplay.hpp"
#include "MP3Display_routines.hpp"

// find . -name .DS_Store -print0 | xargs -0 git rm -f --ignore-unmatch


#define ever (;;)

void MP3Display_routine(){
  // Call corresponding FSM
  (*MP3DisplayState_Routine[mp3display_state])();
}

int main() {

  for ever {
    MP3Display_routine();

    char c;
    printf("Routine end. Any key to continue\n");
    scanf("%c\n", &c);
    if(c == 'e') return 0;
  }
}
