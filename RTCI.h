/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifndef _RTCI_HPP
#define _RTCI_HPP

#include <stdint.h>

#ifndef DEBUG
  #include "stm32l4xx_hal.h"

  extern RTC_HandleTypeDef hrtc;
  extern uint8_t Hour, Sec, Min, Year, Month, Date;
  extern RTC_DateTypeDef RTC_Date;
  extern RTC_TimeTypeDef RTC_Time;

  TIM_HandleTypeDef htim8;

  extern uint32_t track_progress_seconds;
#endif

typedef struct mtime {
  uint8_t sec;
  uint8_t min;
  uint8_t hr;
} mtime;

void RTCI_Init();
void RTCI_getTime(mtime *t);
void RTCI_setTime(const mtime *t);

uint32_t TIMEI_getSongProgress();
void TIMEI_startTimer();
void TIMEI_stopAndResetTimer();

#endif
