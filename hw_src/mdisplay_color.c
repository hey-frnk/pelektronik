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

uint16_t mdisplay_rgb_to565(uint8_t r, uint8_t g, uint8_t b){
  // Mapping goes like RRRRRGGGGGGBBBBB
  return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

static float _mdisplay_hsl_hue2rgb(float p, float q, float t){
  if(t < 0.0) t += 1;
  if(t > 1.0) t -= 1;
  if(t < 0.16666666667f) return p + (q - p) * 6.0f * t;
  if(t < 0.5f) return q;
  if(t < 0.66666666667f) return p + (q - p) * (0.66666666667f - t) * 6.0f;
  return p;
}

uint16_t mdisplay_hsl_to565(uint8_t hue, uint8_t sat, uint8_t lum){
  // https://stackoverflow.com/questions/2353211
  float h = hue / 255.0;
  float s = sat / 255.0;
  float l = lum / 255.0;
  float r, g, b;

  if(s == 0){
      r = l; g = l; b = l; return mdisplay_rgb_to565(255.0 * r, 255.0 * g, 255.0 * b);
  } else{
      float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
      float p = 2.0f * l - q;
      r = _mdisplay_hsl_hue2rgb(p, q, h + 0.333333333333f);
      g = _mdisplay_hsl_hue2rgb(p, q, h);
      b = _mdisplay_hsl_hue2rgb(p, q, h - 0.333333333333f);
  }
  return mdisplay_rgb_to565(255.0 * r, 255.0 * g, 255.0 * b);
}



uint16_t mdisplay_color_wheel(uint8_t phi, uint8_t s, uint8_t l){
  return mdisplay_hsl_to565(phi, s, l);
}
