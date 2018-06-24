#ifdef __cplusplus
extern "C" {
#endif

// MP3 Button Interface functions
// Why didn’t the pointer go to prom?

// This function sets button data to 0b00 state of the provided button
void MP3BI_resetButton(uint8_t button);

// This function retrieves the 2 bit current state of the provided button
inline uint8_t MP3BI_requestState(uint8_t button);

#ifdef __cplusplus
}
#endif

// ...She didn’t have address.      #goVegan #dadProgrammerJokes
