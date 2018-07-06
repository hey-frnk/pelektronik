/*
 * MP3BI.c
 *
 *  Created on: 13.06.2018
 *      Author: Katharina, Luise, Frank Zheng
 */

#include <stdlib.h>
#include <stdint.h>
#include "MP3BI.h"

#define 	_DEBUG_SHORTPRESS 	100
#define		_DEBUG_LONGPRESS		1000

uint8_t buttonRegister = 0;

void _MP3BI_setButton(uint16_t count, uint8_t button) {
	uint8_t mask;
	if(count == 0) mask = BUTTON_STATE_OFF;
	else if(count < 900) mask = BUTTON_STATE_SHORTPRESS;
	else mask = BUTTON_STATE_LONGPRESS;

	buttonRegister |= (mask << (button << 1));
}

void MP3BI_resetButton(uint8_t button) {
	buttonRegister &= ~(0b00000011 << (button << 1));
}

inline uint8_t MP3BI_requestState(uint8_t button) {
	return (buttonRegister >> (button << 1)) & 0b00000011;
}

void MP3BI_retrieveAllButtonStates(uint8_t *b0, uint8_t *b1, uint8_t *b2, uint8_t *b3) {
  *b0 = MP3BI_requestState(BUTTON_LEFT);
  MP3BI_resetButton(BUTTON_LEFT);
  *b1 = MP3BI_requestState(BUTTON_MENU);
  MP3BI_resetButton(BUTTON_MENU);
  *b2 = MP3BI_requestState(BUTTON_BACK);
  MP3BI_resetButton(BUTTON_BACK);
  *b3 = MP3BI_requestState(BUTTON_RIGHT);
  MP3BI_resetButton(BUTTON_RIGHT);
}

void _MP3BI_ci2cl(char input) {
	switch(input) {
		// Left Button
		case 'l': _MP3BI_setButton(_DEBUG_SHORTPRESS, 0); break;			// short press left
		case 'L':	_MP3BI_setButton(_DEBUG_LONGPRESS, 0); break;			// long press left

		// Menu Button
		case 'm': _MP3BI_setButton(_DEBUG_SHORTPRESS, 1); break;
		case 'M': _MP3BI_setButton(_DEBUG_LONGPRESS, 1); break;

		// Back Button
		case 'b': _MP3BI_setButton(_DEBUG_SHORTPRESS, 2); break;
		case 'B': _MP3BI_setButton(_DEBUG_LONGPRESS, 2); break;

		// Right Button
		case 'r': _MP3BI_setButton(_DEBUG_SHORTPRESS, 3); break;
		case 'R': _MP3BI_setButton(_DEBUG_LONGPRESS, 3); break;

		// Terminate this program immediately
		case 'e': exit(0); break;

		default: break;
	}
}
