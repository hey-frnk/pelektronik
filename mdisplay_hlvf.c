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

	uint16_t _global_width = ST7735_LCD_PIXEL_WIDTH;
	uint16_t _global_height = ST7735_LCD_PIXEL_HEIGHT;
	uint16_t rotation = 0;

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

	void st7735_DrawVLine(uint16_t color, uint16_t x, uint16_t y, uint16_t Length){
		(*_crObj.*_st7735_FillScreen)(x, y, 1, Length, ((color & 0xF800) >> 11) * 8.225806451612, 2);
		(*_crObj.*_st7735_FillScreen)(x, y, 1, Length, ((color & 0x07E0) >> 5) * 4.047619047619, 1);
		(*_crObj.*_st7735_FillScreen)(x, y, 1, Length, (color & 0x001F) * 8.225806451612, 0);
	}
#endif


static uint16_t const CWHEEL_LOOKUP[256] = {
	0x78e3, 0x7903, 0x7903, 0x7923, 0x7923, 0x7943, 0x7943, 0x7963, 0x7983, 0x7983, 0x79a3, 0x79a3, 0x79c3, 0x79c3, 0x79e3, 0x79e3,
	0x7a03, 0x7a03, 0x7a23, 0x7a43, 0x7a43, 0x7a63, 0x7a63, 0x7a83, 0x7a83, 0x7aa3, 0x7aa3, 0x7ac3, 0x7ac3, 0x7ae3, 0x7b03, 0x7b03,
	0x7b23, 0x7b23, 0x7b43, 0x7b43, 0x7b63, 0x7b63, 0x7b83, 0x7b83, 0x7ba3, 0x7ba3, 0x7bc3, 0x7bc3, 0x73c3, 0x73c3, 0x73c3, 0x73c3,
	0x6bc3, 0x6bc3, 0x6bc3, 0x6bc3, 0x63c3, 0x63c3, 0x63c3, 0x63c3, 0x5bc3, 0x5bc3, 0x5bc3, 0x53c3, 0x53c3, 0x53c3, 0x53c3, 0x4bc3,
	0x4bc3, 0x4bc3, 0x4bc3, 0x43c3, 0x43c3, 0x43c3, 0x3bc3, 0x3bc3, 0x3bc3, 0x3bc3, 0x33c3, 0x33c3, 0x33c3, 0x33c3, 0x2bc3, 0x2bc3,
	0x2bc3, 0x23c3, 0x23c3, 0x23c3, 0x23c3, 0x1bc3, 0x1bc4, 0x1bc4, 0x1bc4, 0x1bc4, 0x1bc5, 0x1bc5, 0x1bc5, 0x1bc6, 0x1bc6, 0x1bc6,
	0x1bc6, 0x1bc7, 0x1bc7, 0x1bc7, 0x1bc7, 0x1bc8, 0x1bc8, 0x1bc8, 0x1bc9, 0x1bc9, 0x1bc9, 0x1bc9, 0x1bca, 0x1bca, 0x1bca, 0x1bca,
	0x1bcb, 0x1bcb, 0x1bcb, 0x1bcc, 0x1bcc, 0x1bcc, 0x1bcc, 0x1bcd, 0x1bcd, 0x1bcd, 0x1bcd, 0x1bce, 0x1bce, 0x1bce, 0x1bce, 0x1bcf,
	0x1bcf, 0x1baf, 0x1baf, 0x1b8f, 0x1b8f, 0x1b6f, 0x1b6f, 0x1b4f, 0x1b4f, 0x1b2f, 0x1b2f, 0x1b0f, 0x1b0f, 0x1aef, 0x1acf, 0x1acf,
	0x1aaf, 0x1aaf, 0x1a8f, 0x1a8f, 0x1a6f, 0x1a6f, 0x1a4f, 0x1a4f, 0x1a2f, 0x1a0f, 0x1a0f, 0x19ef, 0x19ef, 0x19cf, 0x19cf, 0x19af,
	0x19af, 0x198f, 0x198f, 0x196f, 0x194f, 0x194f, 0x192f, 0x192f, 0x190f, 0x190f, 0x18ef, 0x20ef, 0x20ef, 0x20ef, 0x20ef, 0x28ef,
	0x28ef, 0x28ef, 0x30ef, 0x30ef, 0x30ef, 0x30ef, 0x38ef, 0x38ef, 0x38ef, 0x38ef, 0x40ef, 0x40ef, 0x40ef, 0x48ef, 0x48ef, 0x48ef,
	0x48ef, 0x50ef, 0x50ef, 0x50ef, 0x50ef, 0x58ef, 0x58ef, 0x58ef, 0x60ef, 0x60ef, 0x60ef, 0x60ef, 0x68ef, 0x68ef, 0x68ef, 0x68ef,
	0x70ef, 0x70ef, 0x70ef, 0x70ef, 0x78ef, 0x78ef, 0x78ee, 0x78ee, 0x78ee, 0x78ee, 0x78ed, 0x78ed, 0x78ed, 0x78ed, 0x78ec, 0x78ec,
	0x78ec, 0x78ec, 0x78eb, 0x78eb, 0x78eb, 0x78ea, 0x78ea, 0x78ea, 0x78ea, 0x78e9, 0x78e9, 0x78e9, 0x78e9, 0x78e8, 0x78e8, 0x78e8,
	0x78e7, 0x78e7, 0x78e7, 0x78e7, 0x78e6, 0x78e6, 0x78e6, 0x78e6, 0x78e5, 0x78e5, 0x78e5, 0x78e4, 0x78e4, 0x78e4, 0x78e4, 0x78e3
};

void mdisplay_hlvf_FillScreen(uint16_t color){
	for(uint8_t i = 0; i < _global_height; ++i)
		st7735_DrawHLine(color, 0, i, _global_width);
}

void mdisplay_hlvf_DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color){
	st7735_DrawHLine(color, x, y, width);
	st7735_DrawHLine(color, x, y + height - 1, width);
	st7735_DrawVLine(color, x, y, height);
	st7735_DrawVLine(color, x + width - 1, y, height);
}

void mdisplay_hlvf_FillRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color){
	if(width > height) for(uint8_t i = 0; i < height; ++i) st7735_DrawHLine(color, x, y + i, width);
	else for(uint8_t i = 0; i < width; ++i) st7735_DrawVLine(color, x + i, y, height);
}

static inline void _mdisplay_hlvf_retrieveWidthHeight(uint8_t fontSize, uint8_t *fWidth, uint8_t *fHeight, uint8_t *xspPd, unsigned char **_cptr){
	switch(fontSize){
		case FONT_5X7: 		*fWidth = 5; 		*fHeight = 7; 	*xspPd = 1;		if(_cptr) *_cptr = (unsigned char *)Font5x7;		break;
		case FONT_8X12: 	*fWidth = 8; 		*fHeight = 12; 	*xspPd = 0;		if(_cptr) *_cptr = (unsigned char *)Font8x12; 	break;
		case FONT_8X14: 	*fWidth = 8; 		*fHeight = 14; 	*xspPd = 1;		if(_cptr) *_cptr = (unsigned char *)Font8x14; 	break;
		case FONT_12X16: 	*fWidth = 12;		*fHeight = 16; 	*xspPd = 1;		if(_cptr) *_cptr = (unsigned char *)Font12x16; 	break;
		// case 3: *fWidth = 16; *fHeight = 26; if(_cptr) *_cptr = (unsigned char *)Font16x26; break;
	}
}

void mdisplay_hlvf_DrawChar(uint8_t x, uint8_t y, uint8_t chr, uint16_t color, uint8_t fontSize){
	uint8_t fWidth = 0, fHeight = 0, xspPd;
	unsigned char *_cptr;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, &xspPd, &_cptr);

	// Aaaah, the perks of copying from different libraries!
	if(fontSize == 0){
		uint8_t _ypj;

		// uint8_t buffer[fWidth];			// Width defines font length
		// memcpy(buffer, &_cptr[(chr - 0x20) * fWidth], fWidth);
		uint8_t *buffer = &_cptr[(chr - 0x20) * fWidth];
		for(uint8_t j = 0; j < fHeight; ++j){
			_ypj = y + j;
			for(uint8_t i = 0; i < fWidth; ++i) if((buffer[i] >> j) & 0x01) st7735_WritePixel(x + i, _ypj, color);
		}
	} else {
		uint8_t _ishn, _ypi, _xpfw = x + fWidth, _fwmj, _fwecp = fWidth > 8;

		// Extra sort of alignment?
		uint8_t n = _fwecp ? 1 : 0;
		uint8_t _rshAdj = _fwecp ? 8 : fWidth;

		uint8_t _fHeight = fHeight << n;
		// uint8_t buffer[_fHeight];		// Height defines font length
		// memcpy(buffer, &_cptr[(chr - 0x20) * _fHeight], _fHeight);
		uint8_t *buffer = &_cptr[(chr - 0x20) * _fHeight];

		for(uint8_t i = 0; i < fHeight; ++i){
			_ishn = i << n;
			_ypi = i + y;
			for(uint8_t j = 0; j < _rshAdj; ++j){
				_fwmj = _xpfw - j;
				for(uint8_t k = 0; k <= n; ++k) if((buffer[_ishn + k] >> j) & 0x01) st7735_WritePixel(_fwmj - (k << 3), _ypi, color);
			}
		}
	}
}

void mdisplay_hlvf_DrawString(uint8_t x, uint8_t y, char *str, uint16_t color, uint8_t fontSize, uint8_t alignment){
	// cL tells the length of str
	uint8_t fWidth = 0, fHeight = 0, xspPd = 0;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, &xspPd, NULL);

	// Length of string
	uint16_t cL = strlen(str);

	switch(alignment){
		case ALIGNMENT_CENTER: 	x = x - (((fWidth + xspPd) * cL) >> 1); break;
		case ALIGNMENT_RIGHT:		x = x - ((fWidth + xspPd) * cL); 				break;
		default: break;
	}

	// _x, _y are local backups of X and Y
	uint8_t _x = x; // _y = y;

	while (*str) {
		mdisplay_hlvf_DrawChar(x, y, *str++, color, fontSize);
		// Drawing inside
		if(x < _global_width - ((fWidth + xspPd) << 1)) x += (fWidth + xspPd);
		// Word wrap
		else if (y < _global_height - ((fHeight + 1) << 1)) {x =_x; y += (fHeight + 1);}
		// Reset otherwise
		else {x =_x;} //y =_y;}
	}
}

void mdisplay_hlvf_DrawColorWheelString(uint8_t x, uint8_t y, char *str, uint8_t cStart, uint8_t cEnd, uint8_t s, uint8_t l, uint8_t fontSize, uint8_t alignment){
//void mdisplay_hlvf_DrawColorWheelString(uint8_t x, uint8_t y, char *str, float cStart, float cEnd, float s, float l, uint8_t fontSize, uint8_t alignment){
	// cL tells the length of str, pC is the position counter
	uint8_t fWidth = 0, fHeight = 0, xspPd = 0;
	// float cD = cEnd - cStart;
	uint8_t cD = cEnd - cStart;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, &xspPd, NULL);

	// Length of string
	uint16_t cL = strlen(str), pC = 0;

	switch(alignment){
		case ALIGNMENT_CENTER: 	x = x - (((fWidth + xspPd) * cL) >> 1); break;
		case ALIGNMENT_RIGHT:		x = x - ((fWidth + xspPd) * cL); 				break;
		default: break;
	}

	// _x, _y are local backups of X and Y
	uint8_t _x = x; // _y = y;

	// And draw!
	while (*str){
		mdisplay_hlvf_DrawChar(x, y, *str++, mdisplay_hsl_to565(cStart + (uint8_t)(((uint16_t)cD * ++pC) / (float)cL), s, l), fontSize);
		// Drawing inside
		if(x < _global_width - ((fWidth + xspPd) << 1)) x += (fWidth + xspPd);
		// Word wrap
		else if (y < _global_height - ((fHeight + 1) << 1)) {x =_x; y += (fHeight + 1);}
		// Reset otherwise
		else {x =_x;} // y =_y;}
	}
}

void mdisplay_hlvf_DrawColorWheelStringFast(uint8_t x, uint8_t y, char *str, uint8_t fontSize, uint8_t alignment){
	// cL tells the length of str, pC is the position counter
	uint8_t fWidth = 0, fHeight = 0, xspPd = 0;
	_mdisplay_hlvf_retrieveWidthHeight(fontSize, &fWidth, &fHeight, &xspPd, NULL);

	// Length of string
	uint8_t cL = strlen(str), pC = 0;
	float fT = 255.0f / (float)cL;

	switch(alignment){
		case ALIGNMENT_CENTER: 	x = x - (((fWidth + xspPd) * cL) >> 1); break;
		case ALIGNMENT_RIGHT:		x = x - ((fWidth + xspPd) * cL); 				break;
		default: break;
	}

	// _x, _y are local backups of X and Y
	uint8_t _x = x; // _y = y;

	// And draw!
	while (*str){
		mdisplay_hlvf_DrawChar(x, y, *str++, CWHEEL_LOOKUP[(uint8_t)((float)++pC * fT)], fontSize);
		// Drawing inside
		if(x < _global_width - ((fWidth + xspPd) << 1)) x += (fWidth + xspPd);
		// Word wrap
		else if (y < _global_height - ((fHeight + 1) << 1)) {x =_x; y += (fHeight + 1);}
		// Reset otherwise
		else {x =_x;} // y =_y;}
	}
}

static inline void _mdisplay_hlvf_retrieveIcon(uint8_t size, uint8_t *pixels, unsigned char **_cptr){
	switch(size){
		case HEART16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Heart16x16; break;
		case HEART24: 		*pixels = 24; if(_cptr) *_cptr = (unsigned char *)Heart24x24; break;
		case RABBIT16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Rabbit16x16; break;
		//case LAUGH16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Laugh16x16; break;
		//case MAPLE16: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Maple16x16; break;
		case NAV_PLAY: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Play16x16; break;
		case NAV_PAUSE: 	*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Pause16x16; break;
		case NAV_FWD: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Fwd16x16; break;
		case NAV_RWD: 		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Rwd16x16; break;
		case NAV_SOUND: 	*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Sound16x16; break;
		case NAV_REPA:		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Repeat16x16; break;
		case NAV_REPO:		*pixels = 16; if(_cptr) *_cptr = (unsigned char *)RepeatOnce16x16; break;
		case NAV_SHUFFLE:	*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Shuffle16x16; break;
		case NAV_RECORD:	*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Record16x16; break;
		case NAV_SETTINGS:*pixels = 16; if(_cptr) *_cptr = (unsigned char *)Settings16x16; break;
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
	float _r = r / 255.0f, _g = g / 255.0f, _b = b / 255.0f;
	float _max = max / 255.0f, _min = min / 255.0f;
	float h, s, l = 0.5f * (_max + _min);
	if(max == min) {h = 0; s = 0;}
	else{
		float d = _max - _min;
		s = l > 0.5f ? d / (2.0f - _max - _min) : d / (_max + _min);
		if(max == r) h = (_g - _b) / d + (_g < _b ? 6.0f : 0.0f);
		else if(max == g) h = (_b - _r) / d + 2.0f;
		else if(max == b) h = (_r - _g) / d + 4.0f;
	}

	// Normalize brightness
	uint8_t crmin = _cptr[0], crmax = _cptr[0];
	for(uint16_t i = 0; i < pxl * pxl; ++i){
		if(_cptr[i] < crmin) crmin = _cptr[i];
		if(_cptr[i] > crmax) crmax = _cptr[i];
	}

	// Calculate new (linearized) color mapping
	uint8_t t = 0;
	switch(color){
		case COLOR_BLACK: 	t = 0; 		break;
		case 0x1987: 				t = 64; 	break;
		default:						t = 127; 	break;
	}

	int16_t _t = (int16_t)t;
	float m = (_t - crmax) / (float)(crmin - crmax);
	float n = (crmax * (_t - crmin)) / (float)(crmax - crmin);

	for(uint8_t i = 0; i < pxl; ++i)
		for(uint8_t j = 0; j < pxl; ++j)
			st7735_WritePixel(x + j, y + i, mdisplay_hsl_to565(h * 42.5, s * 255.0, m * (float)(_cptr[pxl * i + j]) + n));

}

















// Go Vegan! (Hey glad you came this far)
