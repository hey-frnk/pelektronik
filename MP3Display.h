#include <stdint.h>

#ifndef MP3DISPLAY_HPP
#define MP3DISPLAY_HPP

typedef struct time {
  uint8_t sec;
  uint8_t min;
  uint8_t hr;
} time;

enum MDISPLAY_TYPE {
  MDISPLAY_TYPE_MENU            = 0,
  MDISPLAY_TYPE_MUSICPLAY       = 1,
  MDISPLAY_TYPE_MUSICEQPLAY     = 2,
  MDISPLAY_TYPE_VOICERECORD     = 3
};

typedef struct MP3Display {
  // Methods
  uint8_t     (*getType)                (void *this);
  void        (*show)                   (void *this);
  void        (*init)                   (struct MP3Display *this);
  void        (*setBatteryState)        (struct MP3Display *this, uint8_t batteryLevel);
  void        (*updateTime)             (struct MP3Display *this, time t);

  // Attributes
  struct MP3Display_vTable              *virtual;
  time                                  t;
  uint8_t                               batteryLevel, type;
} MP3Display;

struct MP3Display_vTable {
  uint8_t     (*getType)                (void *this);
  void        (*show)                   (void *this);
};

void MP3Display_init(MP3Display *this);

#endif
