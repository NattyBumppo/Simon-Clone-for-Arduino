/* 
Pin definitions for Arduino hardware.
 */

#include "pins.h"

// Input

const int SNESPAD_STROBE_PIN = 2; // orange
const int SNESPAD_CLOCK_PIN = 3; // yellow
const int SNESPAD_DATA_PIN =  4; // red

// Difficulty switch -- if the game starts when it's HIGH, it's hard!
const int DIFF_SWITCH_PIN = 13;


// Output
const int RED_LED_PIN = 7;
const int GREEN_LED_PIN = 8;
const int BLUE_LED_PIN = 9;
const int YELLOW_LED_PIN = 10;
const int SPEAKER_PIN = 11;