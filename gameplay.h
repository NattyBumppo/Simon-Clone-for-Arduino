/* 
    Various settings related to gameplay.
 */

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "Arduino.h"
#include "SNESpad.h"
#include "pitches.h"
#include "MelodyPlayer.h"
#include "pins.h"


// Number of successful turns required to win; set when the game starts depending on DIFF_SWITCH_PIN
extern int turnsUntilWin;

extern boolean superHardMode;

// Array to hold colors used so far
extern int colorChain[100];
// Wait this time (ms) between light display; changes depending on difficulty mode
extern int delayBetweenLights;

// LEDs
extern int LEDDisplayTime; // time to show the LED in ms
extern const int FastLEDDisplayTime; // Time to show LED in ms during attract mode 

// Color loop for attract mode
extern const int attractModeColors[30];

// Time to play a Simon Says sound in ms (should probably match LEDDisplayTime)
extern const int soundPlayTime;

extern const int easterEggCheatCode[11];
extern const int easterEggCheatCodeLength;

extern const int redTone;
extern const int greenTone;
extern const int blueTone;
extern const int yellowTone;

// Gameplay elements
extern const int easyModeTurns;
extern const int hardModeTurns;
extern const int superHardModeTurns;

//Controller input method
extern SNESpad snes_pad;

// States for buttons
extern int red_button_state;
extern int green_button_state;
extern int blue_button_state;
extern int yellow_button_state;

// MelodyPlayer object
extern MelodyPlayer melodyPlayer;

#endif