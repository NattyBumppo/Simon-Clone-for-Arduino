/* 
    Various settings related to gameplay.
 */

 #include "gameplay.h"

// Number of successful turns required to win; set when the game starts depending on DIFF_SWITCH_PIN
int turnsUntilWin;

boolean superHardMode = false;

// Array to hold colors used so far
int colorChain[100];
// Wait this time (ms) between light display; changes depending on difficulty mode
int delayBetweenLights = 200;

// LEDs
int LEDDisplayTime = 200; // time to show the LED in ms
const int FastLEDDisplayTime = 120; // Time to show LED in ms during attract mode 

// Color loop for attract mode
const int attractModeColors[30] = { 2, 1, 3, 0, 2, 1, 3, 0, 2, 1, 3, 0, 2, 2, 2, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 2, 2 };

// Time to play a Simon Says sound in ms (should probably match LEDDisplayTime)
const int soundPlayTime = 700;

const int easterEggCheatCode[] = { SNES_UP, SNES_UP, SNES_DOWN, SNES_DOWN, SNES_LEFT, SNES_RIGHT, SNES_LEFT, SNES_RIGHT, SNES_B, SNES_A, SNES_START };
const int easterEggCheatCodeLength = 11;

const int redTone = NOTE_CS6;
const int greenTone = NOTE_D5;
const int blueTone = NOTE_E5;
const int yellowTone = NOTE_B5;

// Gameplay elements
const int easyModeTurns = 7;
const int hardModeTurns = 11;
const int superHardModeTurns = 15;

//Controller input method
SNESpad snes_pad = SNESpad(SNESPAD_STROBE_PIN, SNESPAD_CLOCK_PIN, SNESPAD_DATA_PIN);

// States for buttons
int red_button_state;
int green_button_state;
int blue_button_state;
int yellow_button_state;

// MelodyPlayer object
MelodyPlayer melodyPlayer;