/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 * This is the color library
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MDISPLAY_COLOR_H_
#define MDISPLAY_COLOR_H_

#include <stdint.h>

// Basic RGB 565 Colors
#define   COLOR_WHITE        0xFFFF
#define   COLOR_BLACK        0x0000
#define   COLOR_BLUE         0x001F
#define   COLOR_BRED         0xF81F
#define   COLOR_GRED         0xFFE0
#define   COLOR_GBLUE        0x07FF
#define   COLOR_RED          0xF800
#define   COLOR_MAGENTA      0xF81F
#define   COLOR_GREEN        0x07E0
#define   COLOR_CYAN         0x7FFF
#define   COLOR_YELLOW       0xFFE0
#define   COLOR_BROWN        0xBC40
#define   COLOR_BRRED        0xFC07
#define   COLOR_GRAY         0x8430
#define   COLOR_DARKBLUE     0x01CF
#define   COLOR_LIGHTBLUE    0x7D7C
#define   COLOR_GRAYBLUE     0x5458
#define   COLOR_LIGHTGREEN   0x841F
#define   COLOR_LGRAY        0xC618
#define   COLOR_LGRAYBLUE    0xA651
#define   COLOR_LBBLUE       0x2B12

// RGB 888 (standard) to RGB 565 conversion
extern uint16_t       mdisplay_rgb_to565(uint8_t r, uint8_t g, uint8_t b);

// True HSL (hue, saturation, luminance) to RGB 565 color space transform. All colors are beautiful
uint16_t              mdisplay_hsl_to565(uint8_t h, uint8_t s, uint8_t l);  // h, s, l: [0, 255], fixed point arithmetics
uint16_t              mdisplay_hslT_to565(uint8_t h, uint8_t s, uint8_t l); // damn slow but accurate floating point
// uint16_t              mdisplay_hslf_to565(float h, float s, float l);       // h, s, l: [0, 1] (faster)


#ifdef __cplusplus
}
#endif

#endif
