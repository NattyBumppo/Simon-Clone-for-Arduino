
/*

   Simon Says Project

   Uses a speaker, four LEDs and four switches to play
   a game of "Simon Says" with the user. An additional
   switch can be used for resetting the game.

 */

#include "SNESpad.h"
#include "MelodyPlayer.h"
#include "pins.h"
#include "gameplay.h"
#include "lights.h"
#include "buttonInput.h"

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
}

// Loop through a "attract mode" light pattern until the user hits
// the "start" button, triggering an interrupt
void loop()
{

    int state = 0;
    int lastState = 0;
    int color_i = 0;
    int timesDisplayed = 0;
    superHardMode = false;

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
        }
        else if ((state) && (state ^ easterEggCheatCode[cheatCode_i]))
        {
            // User is pressing a button but not the correct one, so reset the cheat code state
            cheatCode_i = 0;
        }
        // If the user's done with the cheat code, trigger the easter egg
        if (cheatCode_i == easterEggCheatCodeLength)
        {
            melodyPlayer.playEasterEggMelody();
            superHardMode = true;
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
    if (superHardMode)
    {
        turnsUntilWin = superHardModeTurns;
        LEDDisplayTime = 75;
        delayBetweenLights = 75;
    }
    else
    {
        int diffSwitchState = digitalRead(DIFF_SWITCH_PIN);
        if (diffSwitchState == HIGH)
        {
            turnsUntilWin = hardModeTurns;
            LEDDisplayTime = 200;
            delayBetweenLights = 200;
        }
        else
        {
            turnsUntilWin = easyModeTurns;
            LEDDisplayTime = 200;
            delayBetweenLights = 200;
        }    
    }
}


// Main game loop. Returns a boolean that signals whether or not to jump
// straight into the game again (restart) or go into a waiting loop.
void mainGame()
{
    // Set difficulty
    setDifficulty();

    boolean stillPlaying = true;
    boolean wonGame = false;
    int turnNo = 0; // Current player turn
    // Main game loop; loop until the player messes up or wins
    while(stillPlaying && !wonGame)
    {
        // Get a new color and add it to the color chain
        colorChain[turnNo] = randomColor();

        // Play color chain up to this point
        for(int i = 0; i <= turnNo; i++)
        {
            delay(delayBetweenLights);
            playColorAndSound(colorChain[i], LEDDisplayTime, true);
            clearLights();
        }

        // Now, get user input and ensure it matches the pattern
        int correctPresses = 0;
        while ((correctPresses <= turnNo) && stillPlaying)
        {
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
        melodyPlayer.playWinningMelody(turnsUntilWin);
        return;
    }
    // Otherwise, play losing song and go to attract mode loop
    else
    {
        melodyPlayer.playLosingMelody();
        return;
    }   
}

