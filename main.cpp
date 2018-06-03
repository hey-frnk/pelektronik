#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MP3Display.h"
#include "TrackDisplay.h"
#include "TrackEQDisplay.h"
#include "MP3Display_routines.h"

#include "debug/himage.h"
#include "font.h"
#include "mdisplay_color.h"
#include "mdisplay_hlvf.h"

// find . -name .DS_Store -print0 | xargs -0 git rm -f --ignore-unmatch
hImage *_crObj = nullptr;

using namespace std;

#define ever (;;)

void MP3Display_routine(){
  // Call corresponding FSM
  (*MP3DisplayState_Routine[mp3display_state])();
}

int main() {
  #ifdef DEBUG
  _crObj = new hImage(160, 160, "output.bmp");
  #endif

  for ever {
    MP3Display_routine();

    char c;
    printf("Routine end. Any key to continue\n");
    scanf("%c\n", &c);
    if(c == 'e') return 0;
  }
}
