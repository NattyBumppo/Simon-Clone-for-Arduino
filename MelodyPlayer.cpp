/*
   melody.cpp - a class to play various melodies (inspired by the original Super Mario Bros).
 */

#include "Arduino.h"
#include "MelodyPlayer.h"
#include "pitches.h"
#include "pins.h"
#include "gameplay.h"
#include "lights.h"

// Initialize static member variables
const int MelodyPlayer::losingMelodyTempo = 100;
const int MelodyPlayer::losingMelodyLength = 14;
const int MelodyPlayer::losingMelodyTones[14] = { NOTE_C6, 0, NOTE_G5, 0, NOTE_E5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_GS5, NOTE_AS5, NOTE_GS5, NOTE_G5, NOTE_F5, NOTE_G5 };
const int MelodyPlayer::losingMelodyBeats[14] = { 2, 1, 2, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 4 };

const int MelodyPlayer::winningMelodyTempo = 100;
const int MelodyPlayer::winningMelodyLength = 27;
const int MelodyPlayer::winningMelodyTones[27] = { NOTE_G4, NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, NOTE_E6, NOTE_G6, NOTE_E6, NOTE_GS4, NOTE_C5, NOTE_DS5, NOTE_GS5, NOTE_C6, NOTE_DS6, NOTE_GS6, NOTE_DS6, NOTE_AS4, NOTE_D5, NOTE_F5, NOTE_AS5, NOTE_D6, NOTE_F6, NOTE_AS6, NOTE_AS6, NOTE_AS6, NOTE_AS6, NOTE_C7 };
const int MelodyPlayer::winningMelodyBeats[27] = { 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 4 };

const int MelodyPlayer::easterEggMelodyTempo = 100;
const int MelodyPlayer::easterEggMelodyLength = 13;
const int MelodyPlayer::easterEggMelodyTones[13] = { NOTE_E5, NOTE_E5, 0, NOTE_E5, 0, NOTE_C5, NOTE_E5, 0, NOTE_G5, 0, 0, NOTE_G4, 0 };
const int MelodyPlayer::easterEggMelodyBeats[13] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1 };

MelodyPlayer::MelodyPlayer()
{

}

// Plays the specified tone for the specified duration
void MelodyPlayer::playTone(int toneToPlay, int duration)
{
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

void MelodyPlayer::playLosingMelody()
{
    playMelody(losingMelodyTones, losingMelodyBeats, losingMelodyTempo, losingMelodyLength);
}

void MelodyPlayer::playWinningMelody(int turnsAccomplished)
{
    playMelody(winningMelodyTones, winningMelodyBeats, winningMelodyTempo, winningMelodyLength);
    
    // Play fireworks for harder modes
    if (turnsAccomplished == hardModeTurns)
    {
        // Hard mode; play three fireworks sounds
        playFireworks(3);
    }
    else if (turnsAccomplished == superHardModeTurns)
    {
        // Super-hard mode; play six fireworks sounds
        playFireworks(6);
    }
}

// Play a special melody as an easter egg
void MelodyPlayer::playEasterEggMelody()
{
    playMelody(easterEggMelodyTones, easterEggMelodyBeats, easterEggMelodyTempo, easterEggMelodyLength);
}

// Play victory fireworks sounds
void MelodyPlayer::playFireworks(int numFireworks)
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

void MelodyPlayer::playMelody(const int tones[], const int beats[], const int tempo, const int length)
{
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
