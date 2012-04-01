/* 
    Functions to handle button input on the Arduino.
 */
 
#ifndef BUTTONINPUT_H
#define BUTTONINPUT_H

// Returns the button being pressed by the user. If no button
// is being pressed, returns -1.
int getPressedColor();

// Updates the current state for all of the color buttons
void updateButtonStates();

// Checks to see if a certain button is being pressed
boolean isPressed(int);

#endif