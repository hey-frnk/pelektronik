/*
 * mdisplay_hlvf.h
 *
 *  Created on: 30.05.2018
 *      Author: Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MDISPLAY_HLVF_H_
#define MDISPLAY_HLVF_H_

#ifdef DEBUG
  #include <stdint.h>
#endif

// Fill screen with 565 RGB color
void          mdisplay_hlvf_FillScreen(uint16_t color);


// Character / String functions
// Draw character or string. 0: 5x7, 1: 8x14, 2: ...
void          mdisplay_hlvf_DrawChar(uint8_t X, uint8_t Y, uint8_t chr, uint16_t color, uint8_t fontSize);
void          mdisplay_hlvf_DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color, uint8_t fontSize);
// Draw string with color fade (this is damn lit). cStart/cEnd marks the color range (0 - 255), s and l the desired saturation or luminance respectively
void          mdisplay_hlvf_DrawColorWheelString(uint8_t X, uint8_t Y, char *str, uint8_t cStart, uint8_t cEnd, uint8_t s, uint8_t l, uint8_t fontSize);

#endif /* MDISPLAY_HLVF_H_ */

#ifdef __cplusplus
}
#endif
