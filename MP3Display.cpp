#include <stdio.h>
#include "MP3Display.hpp"

void MP3Display::setBatteryState(uint8_t batteryLevel){
  this->batteryLevel = batteryLevel;
  printf("Battery Level Set To: %hhu\n", this->batteryLevel);
}

void MP3Display::updateTime(time t){
  this->t = t;
  printf("Time: %hhuh, %hhum, %hhus\n", this->t.hr, this->t.min, this->t.sec);
}
