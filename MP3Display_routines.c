#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef DEBUG
  #include <iostream>
  #include <string>
  #include <sstream>
  #include <fstream>
  #include "debug/himage.h"

  void transpose(hImage* image1){
    for(int i = 0; i < image1->getWidth(); ++i){
      for(int j = 0; j < i; ++j){
        for(int k = 0; k < 3; ++k){
          uint8_t tmp = image1->getPixel(i, j, k);
          image1->setPixel(i, j, image1->getPixel(j, i, k), k);
          image1->setPixel(j, i, tmp, k);
        }
      }
    }
  }

  // find . -name .DS_Store -print0 | xargs -0 git rm -f --ignore-unmatch
  hImage *_crObj = nullptr;

  using namespace std;
#else
  #include "st7735.h"
  #include "RTCI.h"
#endif

#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"

#include "MP3Display.h"
#include "SDI.h"
#include "MP3DI.h"
#include "TrackDisplay.h"
#include "MenuDisplay.h"
#include "MP3Display_routines.h"

// Active Instance
MP3Display* INSTANCE_Active = NULL;

// Possible allocated instances
TrackDisplay* INSTANCE_TrackDISPLAY = NULL;
MenuDisplay* INSTANCE_TrackListDISPLAY = NULL;
MenuDisplay* INSTANCE_MenuDISPLAY = NULL;

// Current state variable
MP3Display_State mp3display_state = MP3DISPLAYSTATE_BOOT;

void (* MP3DisplayState_Routine[])(void) = { _routine_BOOT,        // Corresponding routines
                                              _routine_PLAY, _routine_TRACKLIST,
                                              _routine_MAINMENU, _routine_SETTINGS,
                                              _routine_RECORD, _routine_SLEEP,
                                              _routine_SHUTDOWN };

const uint32_t menuElementCount = 6;
const char* menuElements[6] = {"Now Playing", "Shuffle All", "Record Voice", "Settings", "Backlight Off", "Shut Down"};
const uint8_t menuElementIconArray[6] = {NAV_PLAY, HEART16, NAV_RECORD, NAV_SETTINGS, NAV_SLEEP, NAV_SHDN};
const uint16_t menuElementColorArray[6] = {COLOR_BLACK, COLOR_RED, COLOR_BLACK, 0x1987, COLOR_YELLOW, COLOR_BRRED};

Track* currentTrack = NULL;

void _routine_BOOT(void){
  #ifdef DEBUG
  _crObj = new hImage(160, 160, "output.bmp");
  #else
  // Initialize Display
  st7735_Init();
  st7735_DisplayOn();
  st7735_setRotation(1);

  // Initialize SD Interface
  SDI_Init();

  // Initialize RTC
  RTCI_Init();
  #endif

  mdisplay_hlvf_FillScreen(COLOR_WHITE);
  mdisplay_hlvf_DrawIcon(35, 47, HEART16, COLOR_GRAY);
  mdisplay_hlvf_DrawString(56, 48, (char *)"welcome.", COLOR_GRAY, FONT_8X14, ALIGNMENT_LEFT);
  mdisplay_hlvf_DrawColorWheelString((_global_width >> 1), 68, (char*)"(C) by LuiseKathiFrank", 200, 255, 255, 127, FONT_5X7, ALIGNMENT_CENTER);

  // Get MP3 Directory
  // SD_FILE_LIST *l = SDI_getFileListFromDirectory("/");
  // TrackList *tl = MP3DI_initTrackListFromFileList(l);
  // SDI_free(l);

  // currentTrack = MP3DI_TrackList_retrieveTrack(tl, 1);

  #ifdef DEBUG
    transpose(_crObj);
    stringstream s;
    s << "FSM_" << (int)mp3display_state << ".bmp";
    _crObj -> setFileName(s.str());
    cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
    transpose(_crObj);

    // Read next state
    char c = 0;
    cout << "Boot Successful. Any key to continue: ";
    cin >> ws;
    cin >> c;

    if(c == 'm') {
      printf("Entering next state (MENU)\n");
      MP3Display_State nextState = MP3DISPLAYSTATE_MAINMENU;
      mp3display_state = nextState;
      (*MP3DisplayState_Routine[nextState])();
    } else if(c == 'p') {
      printf("Entering next state (PLAY)\n");
      MP3Display_State nextState = MP3DISPLAYSTATE_PLAY;
      mp3display_state = nextState;
      (*MP3DisplayState_Routine[nextState])();
    } else if(c == 'f') {
      printf("Entering next state (TRACK_FS_LIST)\n");
      MP3Display_State nextState = MP3DISPLAYSTATE_TRACKLIST;
      mp3display_state = nextState;
      (*MP3DisplayState_Routine[nextState])();
    } else exit(0);
  #else
    HAL_Delay(2000);

    printf("Boot Successful. Entering next state (PLAY)\n");
    MP3Display_State nextState = MP3DISPLAYSTATE_TRACKLIST;
    mp3display_state = nextState;
    (*MP3DisplayState_Routine[nextState])();
  #endif

}

void _routine_PLAY(void){
  // Check for Instance availability
  if(INSTANCE_TrackDISPLAY == NULL){
    // Instance being called for the first time, create standard track display instance
    INSTANCE_TrackDISPLAY = (TrackDisplay *)malloc(sizeof(TrackDisplay));
    TrackDisplay_init(INSTANCE_TrackDISPLAY);

    // Set active instance
    INSTANCE_Active = (MP3Display *)INSTANCE_TrackDISPLAY;

    // Set track information
    if(currentTrack == NULL) return;  // Oh something went wrong
    INSTANCE_TrackDISPLAY->setTrackInfo(INSTANCE_TrackDISPLAY, currentTrack->trackName, currentTrack->artistName, currentTrack->albumName, currentTrack->length);
    INSTANCE_TrackDISPLAY->changeMode(INSTANCE_TrackDISPLAY, TRACKDISPLAY_MODE_SHUFFLE);
  }

  // Display is active
  if(INSTANCE_TrackDISPLAY != NULL){
    // printf("Track Display On. ");
    INSTANCE_TrackDISPLAY->super.show(INSTANCE_TrackDISPLAY);
    // INSTANCE_TrackDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_TrackDISPLAY, 4);
    INSTANCE_TrackDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_TrackDISPLAY, 3);
    INSTANCE_TrackDISPLAY->super.updateTime((MP3Display *)INSTANCE_TrackDISPLAY);
  }

  #ifdef DEBUG
    printf("%d\n", (int)mp3display_state);

    transpose(_crObj);
    stringstream s;
    s << "FSM_" << (int)mp3display_state << ".bmp";
    _crObj -> setFileName(s.str());
    cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
    transpose(_crObj);
  #endif

}

void _routine_TRACKLIST(void){
  // Check for track list menu instance availability
  if(INSTANCE_TrackListDISPLAY == NULL){
    INSTANCE_TrackListDISPLAY = (MenuDisplay *)malloc(sizeof(MenuDisplay));
    MenuDisplay_init(INSTANCE_TrackListDISPLAY);
    INSTANCE_Active = (MP3Display *)INSTANCE_TrackListDISPLAY;

    // Retrieve current directory
    SD_FILE_LIST *l = SDI_getFileListFromDirectory(NULL);
    uint32_t _l_size = l->FILE_LIST_SIZE;
    uint8_t *_iconArr = (uint8_t *)malloc(_l_size * sizeof(uint8_t));
    uint16_t *_colorArr = (uint16_t *)malloc(_l_size * sizeof(uint16_t));
    char **_fNameArr = (char **)malloc(_l_size * sizeof(char *));

    // Icon array
    for(uint32_t i = 0; i < _l_size; ++i) {
      switch(l->FILE_LIST[i]->SD_FILE_TYPE) {
        case TYPE_MP3FILE:    _iconArr[i] = FS_MUSIC;   break;
        case TYPE_BMPIMAGE:   _iconArr[i] = FS_BITMAP;  break;
        case TYPE_DIRECTORY:  _iconArr[i] = FS_DIR;     break;
        default:              _iconArr[i] = FS_UNKNOWN; break;
      }
      _colorArr[i] = COLOR_WHITE;   // We want spacious (wasteful), beautiful icons this time
      _fNameArr[i] = l->FILE_LIST[i]->SD_FILE_NAME;
    }

    INSTANCE_TrackListDISPLAY->updateItems(INSTANCE_TrackListDISPLAY, _fNameArr, _iconArr, _colorArr, _l_size);
  }

  if(INSTANCE_TrackListDISPLAY != NULL) {
    INSTANCE_TrackListDISPLAY->super.show(INSTANCE_TrackListDISPLAY);
    INSTANCE_TrackListDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_TrackListDISPLAY, 2);
    INSTANCE_TrackListDISPLAY->super.updateTime((MP3Display *)INSTANCE_TrackListDISPLAY);
  }

  #ifdef DEBUG
    transpose(_crObj);
    stringstream s;
    s << "FSM_" << (int)mp3display_state << ".bmp";
    _crObj -> setFileName(s.str());
    cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
    transpose(_crObj);

    printf("%d\n", (int)mp3display_state);
    // Read next state
    char c = 0;
    cout << "Main menu. What to do next? (u: up, d: down, p: play): ";
    cin >> ws;
    cin >> c;

    if(c == 'e') exit(0);
    else if(c == 'u'){
      printf("Menu up pressed\n");
      INSTANCE_TrackListDISPLAY->itemUp(INSTANCE_TrackListDISPLAY);
    } else if(c == 'd'){
      printf("Menu down pressed\n");
      INSTANCE_TrackListDISPLAY->itemDown(INSTANCE_TrackListDISPLAY);
    } else if(c == 'p'){
      printf("Entering next state (PLAY)\n");
      MP3Display_State nextState = MP3DISPLAYSTATE_PLAY;
      mp3display_state = nextState;
      (*MP3DisplayState_Routine[nextState])();
    }
  #endif
}

void _routine_MAINMENU(void){
  // Check for main menu instance availability
  if(INSTANCE_MenuDISPLAY == NULL){
    INSTANCE_MenuDISPLAY = (MenuDisplay *)malloc(sizeof(MenuDisplay));
    MenuDisplay_init(INSTANCE_MenuDISPLAY);
    INSTANCE_Active = (MP3Display *)INSTANCE_MenuDISPLAY;

    INSTANCE_MenuDISPLAY->updateItems(INSTANCE_MenuDISPLAY, (char **)menuElements, (uint8_t *)menuElementIconArray, (uint16_t *)menuElementColorArray, menuElementCount);
  }

  if(INSTANCE_MenuDISPLAY != NULL){
    INSTANCE_MenuDISPLAY->super.show(INSTANCE_MenuDISPLAY);
    INSTANCE_MenuDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_MenuDISPLAY, 1);
    INSTANCE_MenuDISPLAY->super.updateTime((MP3Display *)INSTANCE_MenuDISPLAY);
  }

  #ifdef DEBUG
    transpose(_crObj);
    stringstream s;
    s << "FSM_" << (int)mp3display_state << ".bmp";
    _crObj -> setFileName(s.str());
    cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
    transpose(_crObj);

    printf("%d\n", (int)mp3display_state);
    // Read next state
    char c = 0;
    cout << "Main menu. What to do next? (u: up, d: down, p: play): ";
    cin >> ws;
    cin >> c;

    if(c == 'e') exit(0);
    else if(c == 'u'){
      printf("Menu up pressed\n");
      INSTANCE_MenuDISPLAY->itemUp(INSTANCE_MenuDISPLAY);
    } else if(c == 'd'){
      printf("Menu down pressed\n");
      INSTANCE_MenuDISPLAY->itemDown(INSTANCE_MenuDISPLAY);
    } else if(c == 'p'){
      printf("Entering next state (PLAY)\n");
      MP3Display_State nextState = MP3DISPLAYSTATE_PLAY;
      mp3display_state = nextState;
      (*MP3DisplayState_Routine[nextState])();
    }
  #endif
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
