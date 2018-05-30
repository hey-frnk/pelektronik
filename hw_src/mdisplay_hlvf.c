/*
 * mdisplay_hlvf.c
 *
 *  Created on: 30.05.2018
 *      Author: HARDWARECOP
 */

#include <string.h>
#include "st7735.h"
#include "mdisplay_hlvf.h"
#include "font5x7.h"

void mdisplay_hlvf_FillScreen(uint16_t color){
	for(uint8_t i = 0; i < ST7735_LCD_PIXEL_HEIGHT; ++i)
		st7735_DrawHLine(color, 0, i, ST7735_LCD_PIXEL_WIDTH);
}


void mdisplay_hlvf_DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color) {
	uint16_t i, j;
	uint8_t buffer[5];

	memcpy(buffer, &Font5x7[(chr - 32) * 5], 5);

	for(j = 0; j < 7; ++j)
		for(i = 0; i < 5; ++i)
			if((buffer[i] >> j) & 0x01) st7735_WritePixel(X + i, Y + j, color);
}


void mdisplay_hlvf_DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color){
	while (*str) {
		mdisplay_hlvf_DrawChar(X,Y,*str++,color);
	        if (X < ST7735_LCD_PIXEL_WIDTH - 8) { X += 6; } else if (Y < ST7735_LCD_PIXEL_HEIGHT - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
	}
}
