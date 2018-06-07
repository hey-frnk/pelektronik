/*
 *  This is the MP3 Display Color Library
 *  Created by Frank Zheng
 */

#include <math.h>
#include <stdint.h>
#include "mdisplay_color.h"

#ifdef DEBUG
  #include <stdio.h>
  #include <stdint.h>
#endif

inline uint16_t mdisplay_rgb_to565(uint8_t r, uint8_t g, uint8_t b){
  // Mapping goes like RRRRRGGGGGGBBBBB
  return ((r & 0b11111000) << 8) | ((g & 0b11111100) << 3) | (b >> 3);
}

static inline int32_t _mdisplay_fpmul(int32_t x, int32_t y){
  return ((int64_t)x * (int64_t)y) / 0x01000000;
}

static int32_t _mdisplay_hslp_hue2rgb(int32_t p, int32_t q, int32_t t){
  if(t < 0x00000000) t += 0x01000000;
  if(t > 0x01000000) t -= 0x01000000;
  if(t < 0x002AAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), t));
  if(t < 0x00800000) return q;
  if(t < 0x00AAAAAB) return p + _mdisplay_fpmul(0x06000000, _mdisplay_fpmul((q - p), 0x00AAAAAB - t));
  return p;
}

uint16_t mdisplay_hsl_to565(uint8_t h, uint8_t s, uint8_t l){
  int32_t hFP = _mdisplay_fpmul(h << 23, 0x20202);
  int32_t sFP = _mdisplay_fpmul(s << 23, 0x20202);
  int32_t lFP = _mdisplay_fpmul(l << 23, 0x20202);

  int32_t rFP, gFP, bFP;
  if(s == 0) return mdisplay_rgb_to565(l, l, l);
  else{
    int32_t q = (lFP < 0x00800000) ? _mdisplay_fpmul(lFP, 0x01000000 + sFP) : (lFP + sFP - _mdisplay_fpmul(lFP, sFP));
    int32_t p = _mdisplay_fpmul(0x02000000, lFP) - q;
    rFP = _mdisplay_hslp_hue2rgb(p, q, hFP + 0x00555555);
    gFP = _mdisplay_hslp_hue2rgb(p, q, hFP);
    bFP = _mdisplay_hslp_hue2rgb(p, q, hFP - 0x00555555);
  }

  return mdisplay_rgb_to565((_mdisplay_fpmul(0x3FC00000, rFP) >> 22), (_mdisplay_fpmul(0x3FC00000, gFP) >> 22), (_mdisplay_fpmul(0x3FC00000, bFP) >> 22));
}


// LEGACY FUNCTIONS

/*static float _mdisplay_hsl_hue2rgb(float p, float q, float t){
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
}*/

/*uint16_t mdisplay_hslf_to565(float h, float s, float l){
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
}*/
