/*
 * mdisplay_hlvf.h
 *
 *  Created on: 30.05.2018
 *      Author: Frank Zheng
 */

#ifndef MDISPLAY_HLVF_H_
#define MDISPLAY_HLVF_H_

// Fill screen with 565 RGB color
void          mdisplay_hlvf_FillScreen(uint16_t color);


// Character / String functions
// Draw character or string. 0: 5x7, 1: 6x8, 2: ...
void          mdisplay_hlvf_DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color, uint8_t fontSize);
void          mdisplay_hlvf_DrawString5x7(uint8_t X, uint8_t Y, char *str, uint16_t color, uint8_t fontSize);

#endif /* MDISPLAY_HLVF_H_ */
