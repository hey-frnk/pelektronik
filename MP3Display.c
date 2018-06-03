#include <stdio.h>
#include "MP3Display.h"


static uint8_t MP3Display_getType(void *iptr) {
  return ((MP3Display *)iptr)->vt->getType(iptr);
}

static void MP3Display_show(void *iptr) {
  ((MP3Display *)iptr)->vt->show(iptr);
}

static void setBatteryState(MP3Display *iptr, uint8_t batteryLevel) {
  iptr->batteryLevel = batteryLevel;
  printf("Battery Level Set To: %hhu\n", iptr->batteryLevel);
}

static void updateTime(MP3Display *iptr, mtime t) {
  iptr->t = t;
  printf("Time: %hhuh, %hhum, %hhus\n", iptr->t.hr, iptr->t.min, iptr->t.sec);
}

void MP3Display_init(MP3Display *iptr) {
  printf("MP3Display initialized\n");

  iptr->init = MP3Display_init;
  iptr->setBatteryState = setBatteryState;
  iptr->updateTime = updateTime;
  iptr->getType = MP3Display_getType;
  iptr->show = MP3Display_show;
}
