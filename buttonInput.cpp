/* 
    Functions to handle button input on the Arduino.
 */

#include "Arduino.h"
#include "buttonInput.h"
#include "SNESpad.h"
#include "pins.h"
#include "gameplay.h"

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
        return 0;
    }
    else if (green_button_state)
    {
        return 1;
    }
    else if (blue_button_state)
    {
        return 2;
    }
    else if (yellow_button_state)
    {
        return 3;
    }
    else
    {
        return -1;
    }
}

// Updates the current state for all of the color buttons
void updateButtonStates()
{
    int state = snes_pad.buttons();
    red_button_state = state & SNES_A;
    green_button_state = state & SNES_Y;
    blue_button_state = state & SNES_X;
    yellow_button_state = state & SNES_B; 
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
                return true;
            }
            else
            {
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
