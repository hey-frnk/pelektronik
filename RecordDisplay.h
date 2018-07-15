/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in July 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "MP3Display.h"

#ifndef RecordDISPLAY_HPP
#define RecordDISPLAY_HPP

typedef struct RecordDisplay {
  MP3Display                super;

  void        (*startRecording)           (struct RecordDisplay *iptr);
  void        (*stopRecording)            (struct RecordDisplay *iptr);
  void        (*createRecordFile)         (struct RecordDisplay *iptr, char *fileName);

  char                      *fileName;
  uint8_t                   recording, intensityPos;
  uint16_t                  *intensityDisplay;
  uint32_t                  length;
} RecordDisplay;

uint8_t RecordDisplay_getType(void *iptr);
void RecordDisplay_show(void *iptr);
void RecordDisplay_init(RecordDisplay *iptr);
void RecordDisplay_deInit(RecordDisplay *iptr);

#endif

#ifdef __cplusplus
}
#endif
