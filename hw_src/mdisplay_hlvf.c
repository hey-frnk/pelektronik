/*
 * mdisplay_hlvf.c
 *
 *  Created on: 30.05.2018
 *      Author: Frank Zheng
 */

#include <string.h>
#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"

#ifndef DEBUG
	#include "st7735.h"
#else
	// Just for testing
	#include <stdint.h>
	#include "debug/himage.h"

	#define  ST7735_LCD_PIXEL_WIDTH    ((uint16_t)128)
	#define  ST7735_LCD_PIXEL_HEIGHT   ((uint16_t)160)

	extern hImage *_crObj; 

	// Re-reference himage class methods
	void (hImage::*_st7735_WritePixel)(uint32_t, uint32_t, uint8_t, uint8_t, uint8_t) = &hImage::setPixelRGB;
	void (hImage::*_st7735_FillScreen)(uint32_t, uint32_t, uint32_t, uint32_t, uint8_t, uint8_t) = &hImage::setRectangle;

	// Rewrite write pixel low level function
	void st7735_WritePixel(uint16_t X, uint16_t Y, uint16_t color){
		// Convert to RGB 888, write to image instance
		(*_crObj.*_st7735_WritePixel)((uint32_t)X, (uint32_t)Y, ((color & 0xF800) >> 11) * 8.225806451612, ((color & 0x07E0) >> 5) * 4.047619047619, (color & 0x001F) * 8.225806451612);
	}

	// Rewrite write line
	void st7735_DrawHLine(uint16_t color, uint16_t Xpos, uint16_t Ypos, uint16_t Length){
		(*_crObj.*_st7735_FillScreen)(Xpos, Ypos, Length, 1, ((color & 0xF800) >> 11) * 8.225806451612, 2);
		(*_crObj.*_st7735_FillScreen)(Xpos, Ypos, Length, 1, ((color & 0x07E0) >> 5) * 4.047619047619, 1);
		(*_crObj.*_st7735_FillScreen)(Xpos, Ypos, Length, 1, (color & 0x001F) * 8.225806451612, 0);
	}
#endif

void mdisplay_hlvf_FillScreen(uint16_t color){
	for(uint8_t i = 0; i < ST7735_LCD_PIXEL_HEIGHT; ++i)
		st7735_DrawHLine(color, 0, i, ST7735_LCD_PIXEL_WIDTH);
}

static inline void _mdisplay_hlvf_retrieveWidthHeight(uint8_t fontSize, uint8_t *fWidth, uint8_t *fHeight, unsigned char **_cptr){
	switch(fontSize){
		case 0: *fWidth = 5; *fHeight = 7; if(_cptr) *_cptr = (unsigned char *)Font5x7; break;
		case 1: *fWidth = 8; *fHeight = 14; if(_cptr) *_cptr = (unsigned char *)Font8x14; break;
		case 2: *fWidth = 12; *fHeight = 16; if(_cptr) *_cptr = (unsigned char *)Font12x16; break;
		case 3: *fWidth = 16; *fHeight = 26; if(_cptr) *_cptr = (unsigned char *)Font16x26; break;
	}
}

void mdisplay_hlvf_DrawChar(uint8_t X, uint8_t Y, uint8_t chr, uint16_t color, uint8_t fontSize){
	uint8_t fWidth = 0, fHeight = 0;
	unsigned char *_cptr;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, &_cptr);

	// Aaaah, the perks of copying from different libraries!
	if(fontSize == 0){
		uint8_t buffer[fWidth];			// Width defines font length
		memcpy(buffer, &_cptr[(chr - 0x20) * fWidth], fWidth);
		for(uint8_t j = 0; j < fHeight; ++j)
			for(uint8_t i = 0; i < fWidth; ++i)
				if((buffer[i] >> j) & 0x01) st7735_WritePixel(X + i, Y + j, color);
	} else {
		// Extra sort of alignment?
		uint8_t n = (fWidth > 8) ? 1 : 0;
		uint8_t _rshAdj = (fWidth > 8) ? 8 : fWidth;

		uint8_t _fHeight = fHeight << n;
		uint8_t buffer[_fHeight];		// Height defines font length
		memcpy(buffer, &_cptr[(chr - 0x20) * _fHeight], _fHeight);

		for(uint8_t i = 0; i < fHeight; ++i)
			for(uint8_t j = 0; j < _rshAdj; ++j)
				for(uint8_t k = 0; k <= n; ++k)
					if((buffer[(i << n) + k] >> j) & 0x01) st7735_WritePixel(X + (fWidth - j - k * 8), Y + i, color);

	}
}

// uint8_t _fHeight = fHeight << ((fWidth > 8) ? 1 : 0);

void mdisplay_hlvf_DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color, uint8_t fontSize){
	// _X, _Y are local backups of X and Y
	uint8_t fWidth = 0, fHeight = 0, _X = X, _Y = Y;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, NULL);

	while (*str) {
		mdisplay_hlvf_DrawChar(X, Y, *str++, color, fontSize);
		// Drawing inside
		if(X < ST7735_LCD_PIXEL_WIDTH - ((fWidth + 1) << 1)) X += (fWidth + 1);
		// Word wrap
		else if (Y < ST7735_LCD_PIXEL_HEIGHT - ((fHeight + 1) << 1)) {X = _X; Y += (fHeight + 1);}
		// Reset otherwise
		else {X = _X; Y = _Y;}
	}
}

void mdisplay_hlvf_DrawColorWheelString(uint8_t X, uint8_t Y, char *str, uint8_t cStart, uint8_t cEnd, uint8_t s, uint8_t l, uint8_t fontSize){
	// _X, _Y are local backups of X and Y needed for word wrap. cD (cDiff) is the color range
	uint8_t fWidth = 0, fHeight = 0, _X = X, _Y = Y, cD = cEnd - cStart;
	// cL tells the length of str, pC is the position counter
	uint16_t cL = strlen(str), pC = 0;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, NULL);

	// And draw!
	while (*str){
		mdisplay_hlvf_DrawChar(X, Y, *str++, mdisplay_color_wheel((uint8_t)(cStart + (cD / (float)cL) * ++pC), s, l), fontSize);
		// Drawing inside
		if(X < ST7735_LCD_PIXEL_WIDTH - ((fWidth + 1) << 1)) X += (fWidth + 1);
		// Word wrap
		else if (Y < ST7735_LCD_PIXEL_HEIGHT - ((fHeight + 1) << 1)) {X = _X; Y += (fHeight + 1);}
		// Reset otherwise
		else {X = _X; Y = _Y;}
	}
}












// Go Vegan! (Hey glad you came this far)
