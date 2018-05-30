/*
 * mdisplay_hlvf.c
 *
 *  Created on: 30.05.2018
 *      Author: Frank Zheng
 */

#include <string.h>
#include "st7735.h"
#include "mdisplay_hlvf.h"
#include "font5x7.h"
#include "font6x8.h"

void mdisplay_hlvf_FillScreen(uint16_t color){
	for(uint8_t i = 0; i < ST7735_LCD_PIXEL_HEIGHT; ++i)
		st7735_DrawHLine(color, 0, i, ST7735_LCD_PIXEL_WIDTH);
}

void mdisplay_hlvf_DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color, uint8_t fontSize){
	switch(fontSize){
		case 0: mdisplay_hlvf_DrawChar5x7(X, Y, chr, color); break;
		case 1: mdisplay_hlvf_DrawChar6x8(X, Y, chr, color); break;
	}
}

void mdisplay_hlvf_DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color, uint8_t fontSize){
	switch(fontSize){
		case 0: mdisplay_hlvf_DrawString5x7(X, Y, str, color); break;
		case 1: mdisplay_hlvf_DrawString6x8(X, Y, str, color); break;
	}
}

// Begin of internals
// Draw character, 5x7 format
static inline void       mdisplay_hlvf_DrawChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color);
// Draw string, 5x7 format
static inline void       mdisplay_hlvf_DrawString5x7(uint8_t X, uint8_t Y, char *str, uint16_t color);

// Draw character, larger 6x8 format
static inline void       mdisplay_hlvf_DrawChar6x8(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color);
// Draw string, 6x8 format
static inline void       mdisplay_hlvf_DrawString6x8(uint8_t X, uint8_t Y, char *str, uint16_t color);

static inline void mdisplay_hlvf_DrawChar5x7(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color) {
	uint16_t i, j;
	uint8_t buffer[5];

	memcpy(buffer, &Font5x7[(chr - 32) * 5], 5);

	for(j = 0; j < 7; ++j)
		for(i = 0; i < 5; ++i)
			if((buffer[i] >> j) & 0x01) st7735_WritePixel(X + i, Y + j, color);
}

static inline void mdisplay_hlvf_DrawString5x7(uint8_t X, uint8_t Y, char *str, uint16_t color){
	while (*str) {
		mdisplay_hlvf_DrawChar(X,Y,*str++,color);
	        if (X < ST7735_LCD_PIXEL_WIDTH - 8) { X += 6; } else if (Y < ST7735_LCD_PIXEL_HEIGHT - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
	}
}

static inline void mdisplay_hlvf_DrawChar6x8(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color) {
	uint16_t i, j;
	uint8_t buffer[6];

	memcpy(buffer, &Font6x8[(chr - 32) * 6], 6);

	for(j = 0; j < 8; ++j)
		for(i = 0; i < 6; ++i)
			if((buffer[i] >> j) & 0x01) st7735_WritePixel(X + i, Y + j, color);
}

static inline void mdisplay_hlvf_DrawString6x8(uint8_t X, uint8_t Y, char *str, uint16_t color){
	while (*str) {
		mdisplay_hlvf_DrawChar(X,Y,*str++,color);
	        if (X < ST7735_LCD_PIXEL_WIDTH - 8) { X += 7; } else if (Y < ST7735_LCD_PIXEL_HEIGHT - 8) { X = 0; Y += 9; } else { X = 0; Y = 0; }
	}
}
