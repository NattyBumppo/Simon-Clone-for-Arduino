/* 
Pin definitions for Arduino hardware.
 */

#ifndef PINS_H
#define PINS_H

// Input

extern const int SNESPAD_STROBE_PIN; // orange
extern const int SNESPAD_CLOCK_PIN; // yellow
extern const int SNESPAD_DATA_PIN ; // red

// Difficulty switch -- if the game starts when it's HIGH, it's hard!
extern const int DIFF_SWITCH_PIN;


// Output
extern const int RED_LED_PIN;
extern const int GREEN_LED_PIN;
extern const int BLUE_LED_PIN;
extern const int YELLOW_LED_PIN;
extern const int SPEAKER_PIN;

#endif