/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "MP3Display.h"

#ifndef MenuDISPLAY_HPP
#define MenuDISPLAY_HPP

typedef struct MenuDisplay {
  MP3Display                super;

  void        (*updateItems)            (struct MenuDisplay *iptr, char **itemArray, uint8_t *itemIconArray, uint16_t *itemColorArray, uint32_t itemArraySize);
  void        (*itemUp)                 (struct MenuDisplay *iptr);
  void        (*itemDown)               (struct MenuDisplay *iptr);
  void        (*deInit)                 (struct MenuDisplay *iptr);

  uint8_t                   *itemIconArray;
  uint16_t                  *itemColorArray;
  uint32_t                  itemPos, itemArraySize, prevPos;
  char                      **itemArray;
} MenuDisplay;

uint8_t MenuDisplay_getType(void *iptr);
void MenuDisplay_show(void *iptr);
void MenuDisplay_init(MenuDisplay *iptr);

#endif

#ifdef __cplusplus
}
#endif
