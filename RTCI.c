/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#include "RTCI.h"

#ifndef DEBUG
#include "stm32l4xx_hal.h"
#endif

void RTCI_Init() {
  //
}

void RTCI_getTime(mtime *t) {
  #ifndef DEBUG
  HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);

  t->hr = RTC_Time.Hours;
  t->min = RTC_Time.Minutes;
  t->sec = RTC_Time.Seconds;
  #else
  t->hr = 3;
  t->min = 16;
  t->sec = 23;
  #endif
}
