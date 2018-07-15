/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 *
 * TABLE OF CONTENTS. Just type in this to navigate
 * - @search DEBUG_FUNCTIONS
 * - @search ROUTINE_BOOT
 * - @search MSGPROMPTHANDLER
 * - @search ROUTINE_PLAY
 * - @search ROUTINE_TRACKLIST
 * - @search ROUTINE_MAINMENU
 * - @search ROUTINE_SETTINGS
 * - @search ROUTINE_PICTURE
 * - @search ROUTINE_RECORD
 */

// Ein Mathematiker und ein Physiker springen vom Dach. Der Mathematiker fliegt nach oben. Warum?

#define ever (;;) // ROFL

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
// This is the place where all headers meet. WOHOO
#include "MP3Display_routines.h"
#include "MP3Display_Message.h"
#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"
#include "MP3Display.h"
#include "SDI.h"
#include "MP3DI.h"
#include "read_id3/read_id3.h"
#include "read_id3/read_bmp.h"
#include "MP3BI.h"
#include "TrackDisplay.h"
#include "MenuDisplay.h"
#include "RecordDisplay.h"

#ifdef DEBUG
  #include <iostream>
  #include <string>
  #include <sstream>
  #include <fstream>
  #include "debug/himage.h"
#else
  #include "st7735.h"
  #include "RTCI.h"
#endif

// Active Instance
MP3Display        *INSTANCE_Active = NULL;

// Possible allocated instances
TrackDisplay      *INSTANCE_TrackDISPLAY = NULL;
MenuDisplay       *INSTANCE_TrackListDISPLAY = NULL;
MenuDisplay       *INSTANCE_MenuDISPLAY = NULL;
MenuDisplay       *INSTANCE_SettingsDISPLAY = NULL;
RecordDisplay     *INSTANCE_RecordDISPLAY = NULL;

SD_FILE_LIST      *INSTANCE_FILELIST = NULL;

static uint32_t   publicItemPos = 0;

// Current state variable
MP3Display_State mp3display_state = MP3DISPLAYSTATE_BOOT;

void (* MP3DisplayState_Routine[])(void) = { _routine_BOOT,        // Corresponding routines
                                              _routine_PLAY, _routine_TRACKLIST,
                                              _routine_MAINMENU, _routine_SETTINGS,
                                              _routine_PICTURE, _routine_RECORD,
                                              _routine_SLEEP, _routine_SHUTDOWN};

const uint32_t menuElementCount = 6;
const char *menuElements[6] = {"Now Playing", "Shuffle All", "Record Voice", "Settings", "Backlight Off", "Insane Button"};
const uint8_t menuElementIconArray[6] = {NAV_PLAY, HEART16, NAV_RECORD, NAV_SETTINGS, NAV_SLEEP, NAV_SHDN};
const uint16_t menuElementColorArray[6] = {COLOR_BLACK, COLOR_RED, COLOR_BLACK, 0x1987, COLOR_YELLOW, COLOR_BRRED};

const uint32_t settingsElementCount = 2;
const char *settingsElements[2] = {"Brightness", "Time Set"};
const uint8_t settingsElementIconArray[2] = {SET_BRIGHT, SET_TIME};
const uint16_t settingsElementColorArray[2] = {COLOR_BLACK, COLOR_BLACK};

Track* currentTrack = NULL;

/**
  * @brief  Debug functions. Used to render debug
  * @retval None
  * @search DEBUG_FUNCTIONS
  */
// +++++++++++++ DEBUG ONLY FUNCTIONS +++++++++++++
#ifdef DEBUG
  using namespace std;

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

  void _transposePrintImg(MP3Display_State mp3display_state) {
    transpose(_crObj);
    stringstream s;
    s << "FSM_" << (int)mp3display_state << ".bmp";
    _crObj -> setFileName(s.str());
    cout << "Bitmap successfully written to :" << _crObj -> saveAndReturnPath() << "." << endl;
    transpose(_crObj);
  }

  char _readNextState() {
    // Read next state
    char c = 0;
    cout << "State: " << (int)mp3display_state << " looped. Waiting for next action (l/L, m/M, b/B, r/R): ";
    cin >> ws;
    cin >> c;
    return c;
  }
#endif
// +++++++++++++ END OF DEBUG ONLY FUNCTIONS



/**
  * @brief  Boot routine (no instance)
  * @retval None
  * @search ROUTINE_BOOT
  */
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

  #ifdef DEBUG
    _transposePrintImg(mp3display_state);

    // Yup I know now this is super consistent
    cout << "Boot Successful. Automatically entering." << endl;
    printf("Entering next state (PLAY)\n");

  #else
    HAL_Delay(2000);
  #endif

  // Get MP3 Directory
  INSTANCE_FILELIST = SDI_getFileListFromDirectory(NULL);
  if(INSTANCE_FILELIST->FILE_LIST_SIZE == 0) { // No files
    MP3Display zeroInstance; // Prevent null pointer clear
    MP3Display_init(&zeroInstance);
    void (*fileListSizeError[6])(MessagePrompt *, void *) = {MessagePrompt_DrawHandle_DrawMessage, NULL, NULL, NULL, NULL, NULL};
    MsgBox((void *)&zeroInstance, fileListSizeError, MESSAGE_PROMPT_LEFTFOCUSED, (char *)"SD Card Read Error!", (char *)"OK", NULL, 1);
    for ever {
      INSTANCE_FILELIST = SDI_getFileListFromDirectory(NULL);
      if(INSTANCE_FILELIST->FILE_LIST_SIZE > 0) break;
      else MsgBox((void *)&zeroInstance, fileListSizeError, MESSAGE_PROMPT_LEFTFOCUSED, (char *)"SD Card Read Error!", (char *)"OK", NULL, 1);
    }
  }

  if(INSTANCE_FILELIST->FILE_LIST[0]->SD_FILE_TYPE == TYPE_MP3FILE) { // First file is MP3? Great.
    publicItemPos = 0;
    currentTrack = MP3DI_retreiveTrackFromFileName(INSTANCE_FILELIST->FILE_LIST[0]->SD_FILE_NAME);
  } else { // Try finding the first mp3
    uint32_t _bckPos = publicItemPos;
    for ever {
      if(_bckPos < INSTANCE_FILELIST->FILE_LIST_SIZE - 1) ++_bckPos;
      else if(_bckPos == INSTANCE_FILELIST->FILE_LIST_SIZE - 1) _bckPos = 0;

      // Yes, mp3 found
      if(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_TYPE == TYPE_MP3FILE) {
        publicItemPos = _bckPos;
        Track *tr = MP3DI_retreiveTrackFromFileName(INSTANCE_FILELIST->FILE_LIST[publicItemPos]->SD_FILE_NAME);
        if(tr) currentTrack = tr;
        break;
      }
    }
  }

  mdisplay_hlvf_FillRectangle(0, 68, _global_width, 10, COLOR_WHITE);
  mdisplay_hlvf_DrawColorWheelString((_global_width >> 1), 68, (char*)"In A Technicolour Beat", 200, 255, 255, 127, FONT_5X7, ALIGNMENT_CENTER);

  MP3DI_retrieveTrackLength(currentTrack);

  MP3Display_State nextState = MP3DISPLAYSTATE_PLAY;
  mp3display_state = nextState;
  (*MP3DisplayState_Routine[nextState])();
}

/**
  * @brief  Test message prompt handlers, used in debug only
  * @retval None
  * @search MSGPROMPTHANDLER
  */
// Test MsgPrompt Handler
void TestPromptMenuHandler(MessagePrompt *mptr, void *iptr) {
  printf("Trap @Override Menu called!\n");
  // MP3Display *tptr = (MP3Display *)iptr;
  // tptr->clear(tptr);
}

#ifdef DEBUG
void TestPromptInitHandler(MessagePrompt *mptr, void *iptr) {

    // MessagePrompt_Brightness_DrawUpdate(mptr, iptr);
    // MessagePrompt_TimeSet_DrawUpdate(mptr, iptr);
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
}
#else
#define TestPromptInitHandler NULL
#endif

/**
  * @brief  Song playing routine (track display instance)
  * @retval None
  * @search ROUTINE_PLAY
  */
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

    TIMEI_startTimer();
  }

  // Display is active
  if(INSTANCE_TrackDISPLAY != NULL){
    // printf("Track Display On. ");
    INSTANCE_TrackDISPLAY->super.show(INSTANCE_TrackDISPLAY);
    // INSTANCE_TrackDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_TrackDISPLAY, 4);
    INSTANCE_TrackDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_TrackDISPLAY, 3);
    INSTANCE_TrackDISPLAY->super.updateTime((MP3Display *)INSTANCE_TrackDISPLAY);

    // Check for state change
    if(INSTANCE_TrackDISPLAY->status == TRACKDISPLAY_STATUS_ENDED) {
      INSTANCE_TrackDISPLAY->changeStatus(INSTANCE_TrackDISPLAY, TRACKDISPLAY_STATUS_PAUSED);
      _MP3BI_setButton(100, BUTTON_RIGHT);
    }
  }

  #ifdef DEBUG
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
  #endif

  uint8_t btnLeft = 0, btnMenu = 0, btnBack = 0, btnRight = 0;
  MP3BI_retrieveAllButtonStates(&btnLeft, &btnMenu, &btnBack, &btnRight);

  // Skip left or rewind
  if(btnLeft == BUTTON_STATE_SHORTPRESS) {
    uint32_t _bckPos = publicItemPos;
    for ever { // #ProgrammerHumor
      if(_bckPos > 0) --_bckPos;
      else if(_bckPos == 0) _bckPos = INSTANCE_FILELIST->FILE_LIST_SIZE - 1;
      // Yes, mp3 found
      if(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_TYPE == TYPE_MP3FILE) {
        publicItemPos = _bckPos;
        Track *tr = MP3DI_retreiveTrackFromFileName(INSTANCE_FILELIST->FILE_LIST[publicItemPos]->SD_FILE_NAME);
        if(tr) {
          MP3DI_TrackFree(currentTrack);
          currentTrack = tr;
          mdisplay_hlvf_DrawIcon((_global_width >> 1) - 42, 88, NAV_RWD, COLOR_BLACK);
          MP3DI_retrieveTrackLength(currentTrack);
          mdisplay_hlvf_DrawIcon((_global_width >> 1) - 42, 88, NAV_RWD, COLOR_GRAY);
          free(INSTANCE_TrackDISPLAY);
          INSTANCE_TrackDISPLAY = NULL;
          TIMEI_stopAndResetTimer();
        }
        break;
      }
    }
  }

  // Skip this song
  if(btnRight == BUTTON_STATE_SHORTPRESS) {
    uint32_t _bckPos = publicItemPos;
    for ever {
      if(_bckPos < INSTANCE_FILELIST->FILE_LIST_SIZE - 1) ++_bckPos;
      else if(_bckPos == INSTANCE_FILELIST->FILE_LIST_SIZE - 1) _bckPos = 0;
      if(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_TYPE == TYPE_MP3FILE) {
        publicItemPos = _bckPos;
        Track *tr = MP3DI_retreiveTrackFromFileName(INSTANCE_FILELIST->FILE_LIST[publicItemPos]->SD_FILE_NAME);
        if(tr) {
          MP3DI_TrackFree(currentTrack);
          currentTrack = tr;
          mdisplay_hlvf_DrawIcon((_global_width >> 1) + 26, 88, NAV_FWD, COLOR_BLACK); // Draw Skipping
          MP3DI_retrieveTrackLength(currentTrack);
          mdisplay_hlvf_DrawIcon((_global_width >> 1) + 26, 88, NAV_FWD, COLOR_GRAY);
          free(INSTANCE_TrackDISPLAY);
          INSTANCE_TrackDISPLAY = NULL;
          TIMEI_stopAndResetTimer();
        }
        break;
      }
    }
  }

  // Back to file list. Prepare FSM change
  if(btnBack == BUTTON_STATE_LONGPRESS) {
    mp3display_state = MP3DISPLAYSTATE_TRACKLIST;
    free(INSTANCE_TrackDISPLAY);
    MP3DI_TrackFree(currentTrack);
    currentTrack = NULL;
    INSTANCE_TrackDISPLAY = NULL;
    INSTANCE_Active = (MP3Display *)INSTANCE_TrackListDISPLAY;
    TIMEI_stopAndResetTimer();
    // (*MP3DisplayState_Routine[mp3display_state])();
  }

  // Pause track
  if(btnBack == BUTTON_STATE_SHORTPRESS) {
    if(INSTANCE_TrackDISPLAY->status == TRACKDISPLAY_STATUS_PLAYING) {
      TIMEI_stopTimer();
      INSTANCE_TrackDISPLAY->changeStatus(INSTANCE_TrackDISPLAY, TRACKDISPLAY_STATUS_PAUSED);
    } else if(INSTANCE_TrackDISPLAY->status == TRACKDISPLAY_STATUS_PAUSED) {
      TIMEI_startTimer();
      INSTANCE_TrackDISPLAY->changeStatus(INSTANCE_TrackDISPLAY, TRACKDISPLAY_STATUS_PLAYING);
    }

  }
}

/**
  * @brief  File (track) list routine (menu display instance)
  * @retval None
  * @search ROUTINE_TRACKLIST
  */
void _routine_TRACKLIST(void){
  // Check for track list menu instance availability
  if(INSTANCE_TrackListDISPLAY == NULL){
    INSTANCE_TrackListDISPLAY = (MenuDisplay *)malloc(sizeof(MenuDisplay));
    MenuDisplay_init(INSTANCE_TrackListDISPLAY);
    INSTANCE_Active = (MP3Display *)INSTANCE_TrackListDISPLAY;

    if(publicItemPos) INSTANCE_TrackListDISPLAY->itemPos = publicItemPos;

    // Retrieve current directory
    // l = SDI_getFileListFromDirectory(NULL);
    uint32_t _l_size = INSTANCE_FILELIST->FILE_LIST_SIZE;
    uint8_t *_iconArr = (uint8_t *)malloc(_l_size * sizeof(uint8_t));
    uint16_t *_colorArr = (uint16_t *)malloc(_l_size * sizeof(uint16_t));
    char **_fNameArr = (char **)malloc(_l_size * sizeof(char *));

    // Icon array
    for(uint32_t i = 0; i < _l_size; ++i) {
      switch(INSTANCE_FILELIST->FILE_LIST[i]->SD_FILE_TYPE) {
        case TYPE_MP3FILE:    _iconArr[i] = FS_MUSIC;   break;
        case TYPE_BMPIMAGE:   _iconArr[i] = FS_BITMAP;  break;
        case TYPE_DIRECTORY:  _iconArr[i] = FS_DIR;     break;
        default:              _iconArr[i] = FS_UNKNOWN; break;
      }
      _colorArr[i] = COLOR_WHITE;   // We want spacious (wasteful), beautiful icons this time
      _fNameArr[i] = INSTANCE_FILELIST->FILE_LIST[i]->SD_FILE_NAME;
    }

    INSTANCE_TrackListDISPLAY->updateItems(INSTANCE_TrackListDISPLAY, _fNameArr, _iconArr, _colorArr, _l_size);
  }

  if(INSTANCE_TrackListDISPLAY != NULL) {
    INSTANCE_TrackListDISPLAY->super.show(INSTANCE_TrackListDISPLAY);
    INSTANCE_TrackListDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_TrackListDISPLAY, 2);
    INSTANCE_TrackListDISPLAY->super.updateTime((MP3Display *)INSTANCE_TrackListDISPLAY);
  }

  #ifdef DEBUG
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
  #endif

  uint8_t btnLeft = 0, btnMenu = 0, btnBack = 0, btnRight = 0;
  MP3BI_retrieveAllButtonStates(&btnLeft, &btnMenu, &btnBack, &btnRight);

  // One Item Up
  if(btnLeft == BUTTON_STATE_SHORTPRESS) {
    #ifdef DEBUG
    printf("File list menu up pressed\n");
    #endif

    INSTANCE_TrackListDISPLAY->itemUp(INSTANCE_TrackListDISPLAY);
  }

  // One Item Down
  if(btnRight == BUTTON_STATE_SHORTPRESS) {
    #ifdef DEBUG
    printf("File list menu down pressed\n");
    #endif

    INSTANCE_TrackListDISPLAY->itemDown(INSTANCE_TrackListDISPLAY);
  }

  // Play selected song
  if(btnMenu == BUTTON_STATE_SHORTPRESS) {
    // List files once again, Get MP3 Directory
    // INSTANCE_FILELIST = SDI_getFileListFromDirectory(NULL);
    // If valid MP3, set new MP3 file and switch to track playing
    uint32_t itemPos = INSTANCE_TrackListDISPLAY->itemPos;
    publicItemPos = itemPos;

    if(INSTANCE_FILELIST->FILE_LIST[itemPos]->SD_FILE_TYPE == TYPE_MP3FILE) {
      #ifdef DEBUG
      printf("MP3 File Detected!\n");
      #endif

      // TrackList *tl = MP3DI_initTrackListFromFileList(l);
      Track *tr = MP3DI_retreiveTrackFromFileName(INSTANCE_FILELIST->FILE_LIST[itemPos]->SD_FILE_NAME);
      // MP3DI_TrackList_free(tl);
      if(tr) {
        currentTrack = tr;
        MP3DI_retrieveTrackLength(currentTrack);
        mp3display_state = MP3DISPLAYSTATE_PLAY;
        INSTANCE_TrackListDISPLAY->deInit(INSTANCE_TrackListDISPLAY);
        free(INSTANCE_TrackListDISPLAY);
        INSTANCE_TrackListDISPLAY = NULL;
        INSTANCE_Active = (MP3Display *)INSTANCE_TrackDISPLAY;
      }
    } else if(INSTANCE_FILELIST->FILE_LIST[itemPos]->SD_FILE_TYPE == TYPE_BMPIMAGE) {
      #ifdef DEBUG
      printf("Bitmap file detected!\n");
      #endif

      mp3display_state = MP3DISPLAYSTATE_PICTURE;
      INSTANCE_TrackListDISPLAY->deInit(INSTANCE_TrackListDISPLAY);
      free(INSTANCE_TrackListDISPLAY);
      INSTANCE_TrackListDISPLAY = NULL;
      INSTANCE_Active = NULL;

      readbmp_DrawBitmapFromFile(INSTANCE_FILELIST->FILE_LIST[itemPos]->SD_FILE_NAME);
    } else {
      void (*fileTypeErrorMsg[6])(MessagePrompt *, void *) = {MessagePrompt_DrawHandle_DrawMessage, NULL, NULL, NULL, NULL, NULL}; // LOLWUT
      MsgBox((void *)INSTANCE_TrackListDISPLAY, fileTypeErrorMsg, MESSAGE_PROMPT_LEFTFOCUSED, (char *)"File Type Unsupported", (char *)"OK", NULL, 1);
    }
    // SDI_free(l);
  }

  // Back to main menu
  if(btnBack == BUTTON_STATE_SHORTPRESS) {
    // if(l != NULL) SDI_free(l);
    mp3display_state = MP3DISPLAYSTATE_MAINMENU;
    publicItemPos = 0;
    INSTANCE_TrackListDISPLAY->deInit(INSTANCE_TrackListDISPLAY);
    free(INSTANCE_TrackListDISPLAY);
    INSTANCE_TrackListDISPLAY = NULL;
    INSTANCE_Active = (MP3Display *)INSTANCE_MenuDISPLAY;
  }

}

/**
  * @brief  Main menu routine (menu display instance)
  * @retval None
  * @search ROUTINE_MAINMENU
  */
void _routine_MAINMENU(void) {
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
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
  #endif

  uint8_t btnLeft = 0, btnMenu = 0, btnBack = 0, btnRight = 0;
  MP3BI_retrieveAllButtonStates(&btnLeft, &btnMenu, &btnBack, &btnRight);

  // One Item Up
  if(btnLeft == BUTTON_STATE_SHORTPRESS) {
    #ifdef DEBUG
    printf("Main menu up pressed\n");
    #endif

    INSTANCE_MenuDISPLAY->itemUp(INSTANCE_MenuDISPLAY);
  }

  // One Item Down
  if(btnRight == BUTTON_STATE_SHORTPRESS) {
    #ifdef DEBUG
    printf("Main menu down pressed\n");
    #endif

    INSTANCE_MenuDISPLAY->itemDown(INSTANCE_MenuDISPLAY);
  }

  if(btnMenu == BUTTON_STATE_SHORTPRESS) {
    uint32_t itemPos = INSTANCE_MenuDISPLAY->itemPos;
    if(itemPos == 0) { // File list
      mp3display_state = MP3DISPLAYSTATE_TRACKLIST;
      free(INSTANCE_MenuDISPLAY);
      INSTANCE_MenuDISPLAY = NULL;
      INSTANCE_Active = (MP3Display *)INSTANCE_TrackListDISPLAY;
    } else if(itemPos == 2) { // Record
      mp3display_state = MP3DISPLAYSTATE_RECORD;
      free(INSTANCE_MenuDISPLAY);
      INSTANCE_MenuDISPLAY = NULL;
      INSTANCE_Active = (MP3Display *)INSTANCE_RecordDISPLAY;
    } else if(itemPos == 3) { // Settings
      mp3display_state = MP3DISPLAYSTATE_SETTINGS;
      free(INSTANCE_MenuDISPLAY);
      INSTANCE_MenuDISPLAY = NULL;
      INSTANCE_Active = (MP3Display *)INSTANCE_SettingsDISPLAY;
    } else if(itemPos == 5) { // Insane mode
      void (*aaaaaaaaaaaa[6])(MessagePrompt *, void *) = {MessagePrompt_DrawHandle_DrawMessage, NULL, NULL, NULL, NULL, NULL}; // LOLWUT
      MsgBox((void *)INSTANCE_MenuDISPLAY, aaaaaaaaaaaa, MESSAGE_PROMPT_LEFTFOCUSED, (char *)"REALLY INSANE by TheVFDCollective", (char *)"HAHA OK", NULL, 1);
   }
  }
}

/**
  * @brief  Settings menu routine (menu display instance)
  * @retval None
  * @search ROUTINE_SETTINGS
  */
void _routine_SETTINGS(void){
  // Check for main menu instance availability
  if(INSTANCE_SettingsDISPLAY == NULL){
    INSTANCE_SettingsDISPLAY = (MenuDisplay *)malloc(sizeof(MenuDisplay));
    MenuDisplay_init(INSTANCE_SettingsDISPLAY);
    INSTANCE_Active = (MP3Display *)INSTANCE_SettingsDISPLAY;

    INSTANCE_SettingsDISPLAY->updateItems(INSTANCE_SettingsDISPLAY,
                                          (char **)settingsElements,
                                          (uint8_t *)settingsElementIconArray,
                                          (uint16_t *)settingsElementColorArray,
                                          settingsElementCount);
  }

  if(INSTANCE_SettingsDISPLAY != NULL){
    INSTANCE_SettingsDISPLAY->super.show(INSTANCE_SettingsDISPLAY);
    INSTANCE_SettingsDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_SettingsDISPLAY, 2);
    INSTANCE_SettingsDISPLAY->super.updateTime((MP3Display *)INSTANCE_SettingsDISPLAY);
  }

  #ifdef DEBUG
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
  #endif

  uint8_t btnLeft = 0, btnMenu = 0, btnBack = 0, btnRight = 0;
  MP3BI_retrieveAllButtonStates(&btnLeft, &btnMenu, &btnBack, &btnRight);

  // One Item Up
  if(btnLeft == BUTTON_STATE_SHORTPRESS) INSTANCE_SettingsDISPLAY->itemUp(INSTANCE_SettingsDISPLAY);
  // One Item Down
  if(btnRight == BUTTON_STATE_SHORTPRESS) INSTANCE_SettingsDISPLAY->itemDown(INSTANCE_SettingsDISPLAY);

  if(btnMenu == BUTTON_STATE_SHORTPRESS) {
    uint32_t itemPos = INSTANCE_SettingsDISPLAY->itemPos;
    if(itemPos == 0) { // Brightness Set
      void (*BrightnessPromptHandler[6])(MessagePrompt *, void *) = {MessagePrompt_Brightness_DrawUpdate,
                                                                 TestPromptInitHandler,
                                                                 MessagePrompt_Brightness_DrawUpdate,
                                                                 MessagePrompt_Brightness_DrawUpdate,
                                                                 MessagePrompt_Brightness_Menu,
                                                                 MessagePrompt_Brightness_Back};
      MESSAGE_PROMPT_RESULT pres = MsgBox((void *)INSTANCE_SettingsDISPLAY, BrightnessPromptHandler, MESSAGE_PROMPT_SELECTION1, NULL, (char *)"Set", (char *)"Cancel", 3);
      if(pres == MESSAGE_PROMPT_RESULT_OK) {
        // Brightness is ok, do nothing
      } else {
        mdisplay_hlvf_SetBrightness(previousBrightness);
        currentBrightness = 4;
        #ifdef DEBUG
        printf("Brightness restored\n");
        #endif
      }
    } else if(itemPos == 1) {
      void (*TimeSetPromptHandler[6])(MessagePrompt *, void *) = {MessagePrompt_TimeSet_Init,
                                                                  TestPromptInitHandler,
                                                                  MessagePrompt_TimeSet_DrawUpdate,
                                                                  MessagePrompt_TimeSet_DrawUpdate,
                                                                  MessagePrompt_TimeSet_Menu,
                                                                  MessagePrompt_TimeSet_Back};
      MESSAGE_PROMPT_RESULT pres = MsgBox((void *)INSTANCE_SettingsDISPLAY, TimeSetPromptHandler, MESSAGE_PROMPT_SELECTION1, NULL, (char *)"Set", (char *)"Cancel", 4);
      if(pres == MESSAGE_PROMPT_RESULT_OK) {
        // Time ok, do nothing
      } else {
        RTCI_setTime(&tSet);
        RTCI_getTime(&tNew);
        #ifdef DEBUG
        printf("Time restored\n");
        #endif
      }
    }
  }

  if(btnBack == BUTTON_STATE_SHORTPRESS) { // Exit settings
    mp3display_state = MP3DISPLAYSTATE_MAINMENU;
    free(INSTANCE_SettingsDISPLAY);
    INSTANCE_SettingsDISPLAY = NULL;
    INSTANCE_Active = (MP3Display *)INSTANCE_MenuDISPLAY;
  }
}

/**
  * @brief  Picture display routine (no instance)
  * @retval None
  * @search ROUTINE_PICTURE
  */
void _routine_PICTURE(void) {
  #ifdef DEBUG
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
  #endif

  uint8_t btnLeft = 0, btnMenu = 0, btnBack = 0, btnRight = 0;
  MP3BI_retrieveAllButtonStates(&btnLeft, &btnMenu, &btnBack, &btnRight);

  if(btnLeft == BUTTON_STATE_SHORTPRESS) {
    // Try finding the previous picture
    uint32_t _bckPos = publicItemPos;
    for ever { // #ProgrammerHumor
      if(_bckPos > 0) --_bckPos;
      else if(_bckPos == 0) _bckPos = INSTANCE_FILELIST->FILE_LIST_SIZE - 1;

      // Yes, BMP found
      if(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_TYPE == TYPE_BMPIMAGE) {
        publicItemPos = _bckPos;
        mdisplay_hlvf_DrawChar(2, 45, '<', COLOR_WHITE, FONT_8X12);
        readbmp_DrawBitmapFromFile(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_NAME);
        break;
      }
    }
  } else if(btnRight == BUTTON_STATE_SHORTPRESS) {
    // Try finding the next picture
    uint32_t _bckPos = publicItemPos;
    for ever {
      if(_bckPos < INSTANCE_FILELIST->FILE_LIST_SIZE - 1) ++_bckPos;
      else if(_bckPos == INSTANCE_FILELIST->FILE_LIST_SIZE - 1) _bckPos = 0;

      if(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_TYPE == TYPE_BMPIMAGE) {
        publicItemPos = _bckPos;
        mdisplay_hlvf_DrawChar(150, 45, '>', COLOR_WHITE, FONT_8X12);
        readbmp_DrawBitmapFromFile(INSTANCE_FILELIST->FILE_LIST[_bckPos]->SD_FILE_NAME);
        break;
      }
    }
  } else if(btnBack == BUTTON_STATE_SHORTPRESS) {
    mp3display_state = MP3DISPLAYSTATE_TRACKLIST;
    INSTANCE_Active = (MP3Display *)INSTANCE_TrackListDISPLAY;
  }
}

/**
  * @brief  Voice record display routine (no instance)
  * @retval None
  * @search ROUTINE_RECORD
  */
void _routine_RECORD(void){
  if(INSTANCE_RecordDISPLAY == NULL) {
    INSTANCE_RecordDISPLAY = (RecordDisplay *)malloc(sizeof(RecordDisplay));
    RecordDisplay_init(INSTANCE_RecordDISPLAY);
    INSTANCE_Active = (MP3Display *)INSTANCE_RecordDISPLAY;
  }
  // Display is active
  if(INSTANCE_RecordDISPLAY != NULL) {
    INSTANCE_RecordDISPLAY->super.show(INSTANCE_RecordDISPLAY);
    INSTANCE_RecordDISPLAY->super.setBatteryState((MP3Display *)INSTANCE_RecordDISPLAY, 2);
    INSTANCE_RecordDISPLAY->super.updateTime((MP3Display *)INSTANCE_RecordDISPLAY);
  }

  #ifdef DEBUG
    _transposePrintImg(mp3display_state);
    char c = _readNextState();
    _MP3BI_ci2cl(c);
  #endif

  uint8_t btnLeft = 0, btnMenu = 0, btnBack = 0, btnRight = 0;
  MP3BI_retrieveAllButtonStates(&btnLeft, &btnMenu, &btnBack, &btnRight);

  if(btnMenu == BUTTON_STATE_SHORTPRESS) {
    if(INSTANCE_RecordDISPLAY->recording == 0) INSTANCE_RecordDISPLAY->startRecording(INSTANCE_RecordDISPLAY);
    else if(INSTANCE_RecordDISPLAY->recording == 1) INSTANCE_RecordDISPLAY->stopRecording(INSTANCE_RecordDISPLAY);
  }

  if(btnBack == BUTTON_STATE_SHORTPRESS) {

    if(INSTANCE_RecordDISPLAY->recording == 1) { // We are still in recording process.
      void (*RecordStopPrompt[6])(MessagePrompt *, void *) = {MessagePrompt_DrawHandle_DrawMessage, TestPromptInitHandler, NULL, NULL, NULL, NULL};
      MESSAGE_PROMPT_RESULT rspres = MsgBox((void *)INSTANCE_RecordDISPLAY, RecordStopPrompt, MESSAGE_PROMPT_LEFTFOCUSED, (char *)"You're recording. Stop Now?", (char *)"Yes", (char *)"No", 2);
      if(rspres == MESSAGE_PROMPT_RESULT_CANCEL) return; // Don't stop the recording!
    }
    if(INSTANCE_RecordDISPLAY->fileName != NULL) { // If a recoding file has been created
      void (*FileSavePrompt[6])(MessagePrompt *, void *) = {MessagePrompt_DrawHandle_DrawMessage, TestPromptInitHandler, NULL, NULL, NULL, NULL};
      MESSAGE_PROMPT_RESULT rspres = MsgBox((void *)INSTANCE_RecordDISPLAY, FileSavePrompt, MESSAGE_PROMPT_LEFTFOCUSED, (char *)"Save recording 12-23-18_17-46?", (char *)"Yes", (char *)"No", 2);
      if(rspres == MESSAGE_PROMPT_RESULT_OK) {
        // Save File
      } else if(rspres == MESSAGE_PROMPT_RESULT_CANCEL) {
        // Do nothing
      }
    }

    mp3display_state = MP3DISPLAYSTATE_MAINMENU;
    RecordDisplay_deInit(INSTANCE_RecordDISPLAY);
    free(INSTANCE_RecordDISPLAY);
    INSTANCE_RecordDISPLAY = NULL;
    INSTANCE_Active = (MP3Display *)INSTANCE_MenuDISPLAY;
  }
}

void _routine_SLEEP(void){
  printf("Hello7\n");
}

void _routine_SHUTDOWN(void){
  printf("Hello8\n");
}








// Er hat das Vorzeichen verdreht.
// Glad you found this. #goVegan!!
