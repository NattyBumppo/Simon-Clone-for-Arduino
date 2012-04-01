
/*

 Simon Says Project
 
 Uses a speaker, four LEDs and four switches to play
 a game of "Simon Says" with the user. An additional
 switch can be used for resetting the game.
 
 */
 
// To do:
// change resistors
// "more random" melody colors- check!
// "more random" color choices- naaah
// better sampling in attract mode- check!

#include "pitches.h"
#include <SNESpad.h>


// Buttons/switches
const int SNESPAD_STROBE_PIN = 2; // orange
const int SNESPAD_CLOCK_PIN = 3; // yellow
const int SNESPAD_DATA_PIN =  4; // red
SNESpad snes_pad = SNESpad(SNESPAD_STROBE_PIN, SNESPAD_CLOCK_PIN, SNESPAD_DATA_PIN);

int red_button_state;
int green_button_state;
int blue_button_state;
int yellow_button_state;

// LEDs
const int RED_LED_PIN = 7;
const int GREEN_LED_PIN = 8;
const int BLUE_LED_PIN = 9;
const int YELLOW_LED_PIN = 10;
int LEDDisplayTime = 200; // time to show the LED in ms
const int FastLEDDisplayTime = 120; // Time to show LED in ms during attract mode 

// Color loop for attract mode
int attractModeColors[30] = { 2, 1, 3, 0, 2, 1, 3, 0, 2, 1, 3, 0, 2, 2, 2, 2, 0, 3, 1, 2, 0, 3, 1, 2, 0, 3, 1, 2, 2, 2 };

// Difficulty switch -- if the game starts when it's HIGH, it's hard!
const int DIFF_SWITCH_PIN = 13;

// Speaker
const int SPEAKER_PIN = 11;

// Sound-related

// Time to play a Simon Says sound in ms (should probably match LEDDisplayTime)
const int soundPlayTime = 700;

int losingMelodyTones[] = { NOTE_C6, 0, NOTE_G5, 0, NOTE_E5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_GS5, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5, NOTE_G5 };
int losingMelodyBeats[] = { 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 4 };
int losingMelodyTempo = 100;
int losingMelodyLength = 14;

int winningMelodyTones[] = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_E6, NOTE_G6, NOTE_E6, NOTE_GS4, NOTE_C5, NOTE_DS5, NOTE_GS5, NOTE_C6, NOTE_DS6, NOTE_GS6, NOTE_DS6, NOTE_AS4, NOTE_D5, NOTE_F5, NOTE_AS5, NOTE_D6, NOTE_F6, NOTE_AS6, NOTE_AS6, NOTE_AS6, NOTE_AS6, NOTE_C7 };
int winningMelodyBeats[] = { 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 4 };
int winningMelodyTempo = 100;
int winningMelodyLength = 27;

int easterEggCheatCode[] = { SNES_UP, SNES_UP, SNES_DOWN, SNES_DOWN, SNES_LEFT, SNES_RIGHT, SNES_LEFT, SNES_RIGHT, SNES_B, SNES_A, SNES_START };
int easterEggCheatCodeLength = 11;

int easterEggMelodyTones[] = { NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0, NOTE_G5, 0, 0, NOTE_G4, 0 };
int easterEggMelodyBeats[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1 };
int easterEggMelodyTempo = 100;
int easterEggMelodyLength = 13;

char noteNames[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int noteTones[] =  { 139, 147, 165, 175, 196, 220, 247, 262 };
const int redTone = NOTE_CS6;
const int greenTone = NOTE_D5;
const int blueTone = NOTE_E5;
const int yellowTone = NOTE_B5;

// Gameplay elements
// Number of successful turns required to win; set when the game starts depending on DIFF_SWITCH_PIN
int turnsUntilWin;
boolean hardMode = false;
int easyModeTurns = 7;
int hardModeTurns = 11;
int superHardModeTurns = 15;

int colorChain[100]; // Array to hold colors used so far
int delayBetweenLights = 200; // Wait this time (ms) between light display

// Outputs a random integer between 0 and 3 (corresponding to a color)
int randomColor()
{
  return (int)random(0, 4);
}

// Displays a color and plays the corresponding sound
void playColorAndSound(int color, int duration, boolean playAudio)
{
  //Serial.println("playColorAndSound()");
  //Serial.println(yellowTone);

  int toneToPlay;
  // Turn on LED corresponding to the color
  switch (color)
  {
  case 0:
    digitalWrite(RED_LED_PIN, HIGH);
    toneToPlay = redTone;
    break;
  case 1:
    digitalWrite(GREEN_LED_PIN, HIGH);
    toneToPlay = greenTone;
    break;
  case 2:
    digitalWrite(BLUE_LED_PIN, HIGH);
    toneToPlay = blueTone;
    break;
  case 3:
    digitalWrite(YELLOW_LED_PIN, HIGH);
    toneToPlay = yellowTone;
    break;
  default:
    break;
  }

  // Play the corresponding tone for the time in "duration",
  // unless you're not playing audio, in which case just wait
  if (playAudio)
  {
    playTone(toneToPlay, duration);
  }
  else
  {
    delay(duration);
  }


}

// Translates a letter note into a tone and plays it
void playNote(char note, int duration) 
{
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++)
  {
    if (noteNames[i] == note) 
    {
      playTone(noteTones[i], duration);
    }
  }
}

// Plays the specified tone for the specified duration
void playTone(int toneToPlay, int duration) 
{
    Serial.print("Playing ");
    Serial.print(toneToPlay);
    Serial.print("hz for ");
    Serial.print(duration);
    Serial.print("ms...\n");

    // Rest if 0
    if (toneToPlay == 0)
    {
        delay(duration);
    }
    // Otherwise, play
    else
    {
      tone(SPEAKER_PIN, toneToPlay, duration);
      delay(duration * 1.30);
      noTone(SPEAKER_PIN);
    }
}

// Returns the button being pressed by the user. If no button
// is being pressed, returns -1.
int getPressedColor()
{
  // First update buttons states to make sure that we
  // get the most recent information
  updateButtonStates();

  // Now, get the button that's been pressed. If multiple
  // buttons are pressed, get the first one in order.
  if (red_button_state)
  {
    Serial.println("Red pressed!");
    return 0;
  }
  else if (green_button_state)
  {
    Serial.println("Green pressed!");
    return 1;
  }
  else if (blue_button_state)
  {
    Serial.println("Blue pressed!");
    return 2;
  }
  else if (yellow_button_state)
  {
    Serial.println("Yellow pressed!");
    return 3;
  }
  else
  {
    return -1;
  }

}

void playLosingMelody()
{
  Serial.println("Playing losing melody!");
  playMelody(losingMelodyTones, losingMelodyBeats, losingMelodyTempo, losingMelodyLength);
}

void playWinningMelody()
{
  Serial.println("Playing winning melody!");
  playMelody(winningMelodyTones, winningMelodyBeats, winningMelodyTempo, winningMelodyLength);
  // Play fireworks for harder modes
  if (turnsUntilWin == hardModeTurns)
  {
    // Hard mode; play three fireworks sounds
    playFireworks(3);
  }
  else if (turnsUntilWin == superHardModeTurns)
  {
    // Super-hard mode; play six fireworks sounds
    playFireworks(6);
  }
}

// Play victory fireworks sounds
void playFireworks(int numFireworks)
{
  // Firework sound:
  // 133 times a second for .18 seconds, then
  // 190 times a second for .10 seconds, then 
  // 133 times a second for .05 seconds, then
  // 83 times a second, for .05 seconds
  for (int i = 0; i < numFireworks; i++)
  {
    int color = i % 4;
    digitalWrite(RED_LED_PIN + color, HIGH);
    playTone(133, 80);
    playTone(180, 20);
    playTone(170, 20);
    playTone(160, 20);
    playTone(150, 20);
    playTone(140, 20);
    playTone(130, 20);
    playTone(120, 20);
    playTone(110, 20);
    playTone(100, 20);
    playTone(90, 20);
    digitalWrite(RED_LED_PIN + color, LOW);
    delay(200);
  }

  
}


void playMelody(int tones[], int beats[], int tempo, int length)
{
    Serial.print("Playing melody ");
    Serial.print(length);
    Serial.print(" tones long\n");
    int curColor = randomColor();
    int lastColor = curColor;
    for (int i = 0; i < length; i++)
    {
      // Ensure that each color is not the same as the last
      while (curColor == lastColor)
      {
         curColor = randomColor();
      }
      digitalWrite(curColor + RED_LED_PIN, HIGH);
      lastColor = curColor;
      playTone(tones[i], beats[i] * tempo);
      clearLights();
    }
}

// Play the color and sound for a pressed button until the button is released
void playPressedButtonColorAndSound()
{
    Serial.println("playPressedButtonColorAndSound()");
    // Grab first color pressed by the user
    int pressedColor = getPressedColor();

    // Light corresponding light
    lightUpColor(pressedColor);

    // Loop until the user releases that button
    while (isPressed(pressedColor))
    {
        playSoundForColor(pressedColor);
        Serial.println("Looping until user releases...");
        updateButtonStates();
    }
    Serial.println("Button released!");
    noTone(SPEAKER_PIN);
    clearLights();
}

// Lights the LED of the corresponding color
void lightUpColor(int color)
{
    switch (color)
    {
        case 0:
            digitalWrite(RED_LED_PIN, HIGH);
            break;
        case 1:
            digitalWrite(GREEN_LED_PIN, HIGH);
            break;
        case 2:
            digitalWrite(BLUE_LED_PIN, HIGH);
            break;
        case 3:
            digitalWrite(YELLOW_LED_PIN, HIGH);
            break;
        default:
            return;
    }

}

// Plays the appropriate sound given a certain color
void playSoundForColor(int color)
{
    
    // Get frequency in hz
    int frequency;
    switch (color)
    {
        case 0:
            frequency = redTone;
            break;
        case 1:
            frequency = greenTone;
            break;
        case 2:
            frequency = blueTone;
            break;
        case 3:
            frequency = yellowTone;
            break;
        default:
            return;
    }
    // Now, play audio (will be turned off in another function,
    // when the user releases the button)
    tone(SPEAKER_PIN, frequency);
}


// Updates the current state for all of the color buttons
void updateButtonStates()
{
  int state = snes_pad.buttons();
  red_button_state = state & SNES_A;
  green_button_state = state & SNES_Y;
  blue_button_state = state & SNES_X;
  yellow_button_state = state & SNES_B;
 
 /*
 Serial.print("red: ");
 Serial.print(red_button_state);
 Serial.print("\n");
 
 Serial.print("green: ");
 Serial.print(green_button_state);
 Serial.print("\n");
 
 Serial.print("blue: ");
 Serial.print(blue_button_state);
 Serial.print("\n");
 
 Serial.print("yellow: ");
 Serial.print(yellow_button_state);
 Serial.print("\n");
 
 Serial.println("=======");
 */
  
}

// Checks to see if a certain button is being pressed
boolean isPressed(int color)
{
    switch (color)
    {
        case 0:
            if (red_button_state)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case 1:
            if (green_button_state)
            {
                // Serial.println("green still pressed...");
                return true;
            }
            else
            {
                // Serial.println("green not pressed anymore...");
                return false;
            }
            break;
        case 2:
            if (blue_button_state)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        case 3:
            if (yellow_button_state)
            {
                return true;
            }
            else
            {
                return false;
            }
            break;
        default:
            return false;
    }
}

// Turn off all LEDs
void clearLights()
{
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

void setup()
{
  // Input setup (SNESPAD pin modes are set up by library)
  pinMode(DIFF_SWITCH_PIN, INPUT);

  // Set up pin modes for output
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  // Use unconnected analog pin 1 as a random seed
  randomSeed(analogRead(A1));

  // Set up serial input
  Serial.begin(9600);
}

// Play a special melody as an easter egg
void playEasterEggMelody()
{
  Serial.println("Playing easter egg melody...");
  playMelody(easterEggMelodyTones, easterEggMelodyBeats, easterEggMelodyTempo, easterEggMelodyLength);
  
}

// Loop through a "attract mode" light pattern until the user hits
// the "start" button, triggering an interrupt
void loop()
{

  int state = 0;
  int lastState = 0;
  int color_i = 0;
  int timesDisplayed = 0;
  hardMode = false;
  
  // Counter for cheat code state machine
  int cheatCode_i = 0;
  // Loop exits when the player presses start... but not when the current step of the cheat code is looking for a start!
  while(!(state & SNES_START) || (easterEggCheatCode[cheatCode_i] == SNES_START))
  {
    state = snes_pad.buttons();
    
    // Check for button released, and advance cheat code state machine state if necessary
    if ((lastState & easterEggCheatCode[cheatCode_i]) && (!(lastState ^ easterEggCheatCode[cheatCode_i])) && (!state))
    {
      // The correct button was just released, so advance the state
      cheatCode_i++;
      // playTone(NOTE_C4, 100);
      // Serial.println(cheatCode_i);
    }
    else if ((state) && (state ^ easterEggCheatCode[cheatCode_i]))
    {
      // User is pressing a button but not the correct one, so reset the cheat code state
      cheatCode_i = 0;
      // Serial.println(cheatCode_i);
    }
    // If the user's done with the cheat code, trigger the easter egg
    if (cheatCode_i == easterEggCheatCodeLength)
    {
      // Serial.println("Cheat achieved!");
      playEasterEggMelody();
      hardMode = true;
      cheatCode_i = 0;
    }
    
    lastState = state;
    
    // Loop color pattern--display each color for FastLEDDisplayTime ms
    // (broken into 32 parts, to increase sampling rate)
    playColorAndSound(attractModeColors[color_i], FastLEDDisplayTime / 32, false);
    clearLights();
    timesDisplayed++;
    if (timesDisplayed > 31)
    {
      timesDisplayed = 0;
      color_i++;
    }
    if (color_i >= 30)
    {
      color_i = 0; 
    }
  }
  // Start was pressed, so start the game
  delay(200);
  mainGame();
}

// Sets the difficulty depending on the state of DIFF_SWITCH_PIN
void setDifficulty()
{
  if (hardMode)
  {
    Serial.println("Setting to super-hard mode!");
    turnsUntilWin = superHardModeTurns;
    LEDDisplayTime = 75;
    delayBetweenLights = 75;
  }
  else
  {
    int diffSwitchState = digitalRead(DIFF_SWITCH_PIN);
    if (diffSwitchState == HIGH)
    {
      Serial.println("Setting to hard mode.");
      turnsUntilWin = hardModeTurns;
      LEDDisplayTime = 200;
      delayBetweenLights = 200;
    }
    else
    {
      turnsUntilWin = easyModeTurns;
      Serial.println("Setting to easy mode.");
      LEDDisplayTime = 200;
      delayBetweenLights = 200;
    }    
  }
}


// Main game loop. Returns a boolean that signals whether or not to jump
// straight into the game again (restart) or go into a waiting loop.
void mainGame()
{
  // Serial.println("Entering main game...");
  // Set difficulty
  setDifficulty();
  
  boolean stillPlaying = true;
  boolean wonGame = false;
  int turnNo = 0; // Current player turn
  // Main game loop; loop until the player messes up or wins
  while(stillPlaying && !wonGame)
  {
    // Serial.println("Main game looping...");
    // Get a new color and add it to the color chain
    colorChain[turnNo] = randomColor();

    // Play color chain up to this point
    for(int i = 0; i <= turnNo; i++)
    {
      delay(delayBetweenLights);
      Serial.println(colorChain[i]);
      playColorAndSound(colorChain[i], LEDDisplayTime, true);
      clearLights();
    }

    // Now, get user input and ensure it matches the pattern
    int correctPresses = 0;
    while ((correctPresses <= turnNo) && stillPlaying)
    {
      // Serial.println("Looping to get user input...");
  
      // If the user is pressing a color button,
      // show its sound and light until they release the button
      int pressedColor = getPressedColor();

      if (pressedColor != -1)
      {
        // Play color and sound
        playPressedButtonColorAndSound();

        // Check if correct
        if (pressedColor == colorChain[correctPresses])
        {
          correctPresses++;
        }
        else
        {
            // Screwed up, so the player loses
          stillPlaying = false;
        }
      
      }
    }

    // At this point, the user has successfully pressed all
    // of the color chain up to this point
    delay(400);

    if (correctPresses == turnsUntilWin)
    {
      // User has won, so exit!
      wonGame = true;
    }

    // Increment turn number
    turnNo++;
  }

  // If user won, play victory song and go to attract mode loop
  if (wonGame)
  {
    playWinningMelody();
    return;
  }
  // Otherwise, play losing song and go to attract mode loop
  else
  {
    playLosingMelody();
    return;
  }   
}

