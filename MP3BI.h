#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern uint16_t count;
extern uint8_t buttonRegister;

#define 	BUTTON_STATE_OFF           0b00000000
#define 	BUTTON_STATE_SHORTPRESS    0b00000001
#define 	BUTTON_STATE_LONGPRESS     0b00000010

#define   BUTTON_LEFT          0
#define   BUTTON_MENU          1
#define   BUTTON_BACK          2
#define   BUTTON_RIGHT         3

// MP3 Button Interface functions

// This function sets button data to 0b00 state of the provided button
void      MP3BI_resetButton(uint8_t button);

// This function retrieves the 2 bit current state of the provided button
uint8_t   MP3BI_requestState(uint8_t button);

// Some internal functions
void      _MP3BI_setButton(uint16_t, uint8_t);

// Char input to button count length function
void      _MP3BI_ci2cl(char);

#ifdef __cplusplus
}
#endif
