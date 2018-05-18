#ifndef MP3DISPLAY_HPP
#define MP3DISPLAY_HPP

#include <stdint.h>

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

class MP3Display {

protected:
  time            t;
  uint8_t         batteryLevel, type;

public:
  virtual uint8_t getType() = 0;
  virtual void    show() = 0;
  void            setBatteryState(uint8_t);
  void            updateTime(time);

};

#endif
