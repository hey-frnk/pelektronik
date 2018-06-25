/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef MP3DISPLAY_HPP
#define MP3DISPLAY_HPP

enum MDISPLAY_TYPE {
  MDISPLAY_TYPE_MENU            = 0,
  MDISPLAY_TYPE_MUSICPLAY       = 1,
  MDISPLAY_TYPE_MUSICEQPLAY     = 2,
  MDISPLAY_TYPE_VOICERECORD     = 3
};

typedef struct MP3Display {
  // Methods
  uint8_t     (*getType)                (void *iptr);
  void        (*show)                   (void *iptr);
  void        (*init)                   (struct MP3Display *iptr);
  void        (*setBatteryState)        (struct MP3Display *iptr, uint8_t batteryLevel);
  void        (*updateTime)             (struct MP3Display *iptr);

  // Attributes
  struct MP3Display_vTable              *vt;
  // mtime                                 t;
  uint8_t                               batteryLevel, type;
} MP3Display;

struct MP3Display_vTable {
  uint8_t     (*getType)                (void *iptr);
  void        (*show)                   (void *iptr);
};

void MP3Display_init(MP3Display *iptr);

#endif

#ifdef __cplusplus
}
#endif
