/*
 *  This is the MP3 Display Color Library
 *  Created by Frank Zheng
 */

#include <math.h>
#include "mdisplay_color.h"

#ifdef DEBUG
  #include <stdio.h>
  #include <stdint.h>
#endif

inline uint16_t mdisplay_rgb_to565(uint8_t r, uint8_t g, uint8_t b){
  // Mapping goes like RRRRRGGGGGGBBBBB
  return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

uint16_t mdisplay_hsl_to565(uint8_t h, uint8_t s, uint8_t l){
  // Fast conversion algorithm, see https://stackoverflow.com/questions/13105185
  uint8_t  r, g, b, lo, c, x, m;
  uint16_t h1, l1, H;
  l1 = l + 1;
  if (l < 128) c = ((l1 << 1) * s) >> 8;
  else c = (512 - (l1 << 1)) * s >> 8;

  H = h * 6;      // 0 to 1535 (actually 1530)
  lo = H & 255;   // Low byte  = primary/secondary color mix
  h1 = lo + 1;

  // even sextant, like red to yellow
  if ((H & 256) == 0) x = h1 * c >> 8;
  // odd sextant, like yellow to green
  else x = (256 - h1) * c >> 8;

  m = l - (c >> 1);
  switch(H >> 8) {       // High byte = sextant of colorwheel
    case 0 : r = c  ; g = x  ; b = 0  ; break; // R to Y
    case 1 : r = x  ; g = c  ; b = 0  ; break; // Y to G
    case 2 : r = 0  ; g = c  ; b = x  ; break; // G to C
    case 3 : r = 0  ; g = x  ; b = c  ; break; // C to B
    case 4 : r = x  ; g = 0  ; b = c  ; break; // B to M
    default: r = c  ; g = 0  ; b = x  ; break; // M to R
  }

  // 888 to 565 color
  return mdisplay_rgb_to565(r, g, b);
}

uint16_t mdisplay_color_wheel(uint8_t phi, uint8_t s, uint8_t l){
  return mdisplay_hsl_to565(phi, s, l);
}
