#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "MP3Display.h"
#include "TrackDisplay.h"
#include "MP3Display_routines.h"

#include "debug/himage.h"
#include "font.h"
#include "mdisplay_color.h"
#include "mdisplay_hlvf.h"

using namespace std;

#define ever (;;)

void MP3Display_routine(){
  // Call corresponding FSM
  (*MP3DisplayState_Routine[mp3display_state])();

}

int main() {

  for ever {
    MP3Display_routine();

    /*char c = 0;
    cout << "All end (for real). Any key to continue: ";
    cin >> ws;
    cin >> c;

    if(c == 'e') exit(0);*/
  }


}
