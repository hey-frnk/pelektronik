/*
 * Part of the MP3 Player project PJElektronikSoSe2018
 *
 *  Created in May/June 2018
 *      Author: Copyright (C) Frank Zheng
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MDISPLAY_HLVF_H_
#define MDISPLAY_HLVF_H_

#include <stdint.h>

#ifdef DEBUG
	#define  ST7735_LCD_PIXEL_WIDTH    ((uint16_t)160)
	#define  ST7735_LCD_PIXEL_HEIGHT   ((uint16_t)128)
  extern uint16_t _global_width;
  extern uint16_t _global_height;
  extern uint16_t rotation;
#else
	#include "stm32l4xx_hal.h"
	extern TIM_HandleTypeDef htim3;
#endif

// Set Brightness (Internally transformed to PWM duty cycle)
// 0: very dark, 1: dark, 2: normal, 3: bright, 4: very bright (default, VCC)
void 					mdisplay_hlvf_SetBrightness(uint8_t brightnessLevel);

// Geometry functions
void          mdisplay_hlvf_FillScreen(uint16_t color);
void          mdisplay_hlvf_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);
void   		    mdisplay_hlvf_FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color);

// Character / String functions
// Draw character or string. 0: 5x7, 1: 8x14, 2: ...
void          mdisplay_hlvf_DrawChar(uint8_t x, uint8_t y, uint8_t chr, uint16_t color, uint8_t fontSize);
void          mdisplay_hlvf_DrawString(uint8_t x, uint8_t y, const char *str, uint16_t color, uint8_t fontSize, uint8_t alignment);
// Draw string with color fade (this is damn lit). cStart/cEnd marks the color range (0 - 255), s and l the desired saturation or luminance respectively
void          mdisplay_hlvf_DrawColorWheelString(uint8_t x, uint8_t y, const char *str, uint8_t cStart, uint8_t cEnd, uint8_t s, uint8_t l, uint8_t fontSize, uint8_t alignment);
void          mdisplay_hlvf_DrawColorWheelStringFast(uint8_t x, uint8_t y, const char *str, uint8_t fontSize, uint8_t alignment);

void 					mdisplay_hlvf_DrawTwoLineString(uint8_t x, uint8_t y, const char *str, uint16_t color, uint8_t fontSize, uint8_t alignment, uint8_t lineCharCount);

// Draw shape
void          mdisplay_hlvf_DrawIcon(uint8_t x, uint8_t y, uint8_t size, uint16_t color);
void 					mdisplay_hlvf_DrawColorIcon(uint8_t x, uint8_t y, uint8_t instance, uint8_t grayFlag);

#endif /* MDISPLAY_HLVF_H_ */

#ifdef __cplusplus
}
#endif
