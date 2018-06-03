/*
 * mdisplay_hlvf.c
 *
 *  Created on: 30.05.2018
 *      Author: Frank Zheng
 */

#include <string.h>
#include <math.h>
#include "mdisplay_hlvf.h"
#include "mdisplay_color.h"
#include "font.h"

#define D_PI 6.28318530718

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

	// Rewrite write pixel low level function: Convert to RGB 888, write to image instance
	#define st7735_WritePixel(x, y, color) \
		(*_crObj.*_st7735_WritePixel)((uint32_t)(x), (uint32_t)(y), (((color) & 0xF800) >> 11) * 8.225806451612, (((color) & 0x07E0) >> 5) * 4.047619047619, ((color) & 0x001F) * 8.225806451612)

	// Rewrite write line
	void st7735_DrawHLine(uint16_t color, uint16_t x, uint16_t y, uint16_t Length){
		(*_crObj.*_st7735_FillScreen)(x, y, Length, 1, ((color & 0xF800) >> 11) * 8.225806451612, 2);
		(*_crObj.*_st7735_FillScreen)(x, y, Length, 1, ((color & 0x07E0) >> 5) * 4.047619047619, 1);
		(*_crObj.*_st7735_FillScreen)(x, y, Length, 1, (color & 0x001F) * 8.225806451612, 0);
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

void mdisplay_hlvf_DrawChar(uint8_t x, uint8_t y, uint8_t chr, uint16_t color, uint8_t fontSize){
	uint8_t fWidth = 0, fHeight = 0;
	unsigned char *_cptr;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, &_cptr);

	// Aaaah, the perks of copying from different libraries!
	if(fontSize == 0){
		uint8_t buffer[fWidth];			// Width defines font length
		memcpy(buffer, &_cptr[(chr - 0x20) * fWidth], fWidth);
		for(uint8_t j = 0; j < fHeight; ++j)
			for(uint8_t i = 0; i < fWidth; ++i)
				if((buffer[i] >> j) & 0x01) st7735_WritePixel(x + i, y + j, color);
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
					if((buffer[(i << n) + k] >> j) & 0x01) st7735_WritePixel(x + (fWidth - j - k * 8), y + i, color);

	}
}

void mdisplay_hlvf_DrawString(uint8_t x, uint8_t y, char *str, uint16_t color, uint8_t fontSize){
	// _x, _y are local backups of X and Y
	uint8_t fWidth = 0, fHeight = 0, _x = x, _y = y;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, NULL);

	while (*str) {
		mdisplay_hlvf_DrawChar(x, y, *str++, color, fontSize);
		// Drawing inside
		if(x < ST7735_LCD_PIXEL_WIDTH - ((fWidth + 1) << 1)) x += (fWidth + 1);
		// Word wrap
		else if (y < ST7735_LCD_PIXEL_HEIGHT - ((fHeight + 1) << 1)) {x =_x; y += (fHeight + 1);}
		// Reset otherwise
		else {x =_x; y =_y;}
	}
}

void mdisplay_hlvf_DrawColorWheelString(uint8_t x, uint8_t y, char *str, uint8_t cStart, uint8_t cEnd, uint8_t s, uint8_t l, uint8_t fontSize){
	// _x, _y are local backups of X and Y needed for word wrap. cD (cDiff) is the color range
	uint8_t fWidth = 0, fHeight = 0, _x = x, _y = y, cD = cEnd - cStart;
	// cL tells the length of str, pC is the position counter
	uint16_t cL = strlen(str), pC = 0;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, NULL);

	// And draw!
	while (*str){
		mdisplay_hlvf_DrawChar(x, y, *str++, mdisplay_color_wheel((uint8_t)(cStart + (cD / (float)cL) * ++pC), s, l), fontSize);
		// Drawing inside
		if(x < ST7735_LCD_PIXEL_WIDTH - ((fWidth + 1) << 1)) x += (fWidth + 1);
		// Word wrap
		else if (y < ST7735_LCD_PIXEL_HEIGHT - ((fHeight + 1) << 1)) {x =_x; y += (fHeight + 1);}
		// Reset otherwise
		else {x =_x; y =_y;}
	}
}

static inline void _mdisplay_hlvf_retrieveIcon(uint8_t size, uint8_t *pixels, unsigned char **_cptr){
	switch(size){
		case HEART16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Heart16x16; break;
		case HEART24: 		*pixels = 24; if(_cptr) *_cptr = (unsigned char *)Heart24x24; break;
		case RABBIT16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Rabbit16x16; break;
		case LAUGH16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Laugh16x16; break;
		case MAPLE16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Maple16x16; break;
	}
}

// Draw shape
void mdisplay_hlvf_DrawIcon(uint8_t x, uint8_t y, uint8_t size, uint16_t color){
	uint8_t pxl = 24;
	unsigned char *_cptr;
	_mdisplay_hlvf_retrieveIcon(size, &pxl, &_cptr);

	// Retrieve RGB value, find max, min. Begin of internal HSL transform
	uint8_t r = 8.2258064516129032258f * (color >> 11);
	uint8_t g = 4.0476190476190476190f * ((color >> 5) & 0b00111111);
	uint8_t b = 8.2258064516129032258f * (color & 0b00011111);
	uint8_t max = r > g ? (r > b ? r : b) : (g > b ? g : b);
	uint8_t min = r < g ? (r < b ? r : b) : (g < b ? g : b);
	float _r = r / 255.0, _g = g / 255.0, _b = b / 255.0;
	float _max = max / 255.0, _min = min / 255.0;
	float h, s, l = 0.5 * (_max + _min);
	if(max == min) {h = 0; s = 0;}
	else{
		float d = _max - _min;
    s = l > 0.5 ? d / (2.0 - _max - _min) : d / (_max + _min);
    if(max == r) h = (_g - _b) / d + (_g < _b ? 6.0 : 0.0);
		else if(max == g) h = (_b - _r) / d + 2.0;
    else if(max == b) h = (_r - _g) / d + 4.0;
	}
	// End of internal HSL transform

	for(uint8_t i = 0; i < pxl; ++i)
		for(uint8_t j = 0; j < pxl; ++j)
			st7735_WritePixel(x + j, y + i, mdisplay_hsl_to565(h * 42.5, s * 255.0, _cptr[pxl * i + j]));

}

















// Go Vegan! (Hey glad you came this far)
