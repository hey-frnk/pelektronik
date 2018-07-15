/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#include "RTCI.h"

#ifndef DEBUG
  #include "stm32l4xx_hal.h"
#else
  #include <stdio.h>
  uint32_t track_progress_seconds = 64;
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

void RTCI_setTime(const mtime *t) {
  #ifndef DEBUG
  RTC_Time.Hours = t->hr;
  RTC_Time.Minutes = t->min;
  RTC_Time.Seconds = 0;
  HAL_RTC_SetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
  #else
  printf("Time Set to %hhu: %hhu\n", t->hr, t->min);
  #endif
}

uint32_t TIMEI_getSongProgress() {
  return track_progress_seconds;
}

void TIMEI_startTimer() {
  #ifndef DEBUG
  HAL_TIM_Base_Start_IT(&htim8);
  #endif
}

void TIMEI_stopTimer() {
  #ifndef DEBUG
  HAL_TIM_Base_Stop_IT(&htim8);
  #endif
}

void TIMEI_stopAndResetTimer() {
  #ifndef DEBUG
  HAL_TIM_Base_Stop_IT(&htim8);
  #endif
  track_progress_seconds = 0;
}
