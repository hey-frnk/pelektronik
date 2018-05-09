#include <stdio.h>
#include "MP3Display.h"


static uint8_t MP3Display_getType(void *this) {
  return ((MP3Display *)this)->virtual->getType(this);
}

static void MP3Display_show(void *this) {
  ((MP3Display *)this)->virtual->show(this);
}

static void setBatteryState(MP3Display *this, uint8_t batteryLevel) {
  this->batteryLevel = batteryLevel;
  printf("Battery Level Set To: %hhu\n", this->batteryLevel);
}

static void updateTime(MP3Display *this, time t) {
  this->t = t;
  printf("Time: %hhuh, %hhum, %hhus\n", this->t.hr, this->t.min, this->t.sec);
}

void MP3Display_init(MP3Display *this) {
  printf("MP3Display initialized\n");

  this->init = MP3Display_init;
  this->setBatteryState = setBatteryState;
  this->updateTime = updateTime;
  this->getType = MP3Display_getType;
  this->show = MP3Display_show;
}
