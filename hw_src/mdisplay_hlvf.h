/*
 * mdisplay_hlvf.h
 *
 *  Created on: 30.05.2018
 *      Author: HARDWARECOP
 */

#ifndef MDISPLAY_HLVF_H_
#define MDISPLAY_HLVF_H_


void     		mdisplay_hlvf_FillScreen(uint16_t color);
void	 		mdisplay_hlvf_DrawChar(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color);
void 	 		mdisplay_hlvf_DrawString(uint8_t X, uint8_t Y, char *str, uint16_t color);

#endif /* MDISPLAY_HLVF_H_ */
