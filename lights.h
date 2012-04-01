/* 
    Functions to manipulate lights on the Arduino. Some sound-related code is also included.
 */

#ifndef LIGHTS_H
#define LIGHTS_H

// Outputs a random integer between 0 and 3 (corresponding to a color)
int randomColor();

// Displays a color and plays the corresponding sound
void playColorAndSound(int, int, boolean);

// Lights the LED of the corresponding color
void lightUpColor(int);

// Turn off all LEDs
void clearLights();

// Plays the appropriate sound given a certain color
void playSoundForColor(int);

// Play the color and sound for a pressed button until the button is released
void playPressedButtonColorAndSound();

#endif