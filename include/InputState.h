#ifndef INPUTSTATE_H
#define INPUTSTATE_H

struct InputState
{
    bool forward = false;
    bool left = false;
    bool back = false;
    bool right = false;
    int mLocationX = 0;
    int mLocationY = 0;
    bool primarySpellPressed = false;
    bool secondarySpellPressed = false;
    bool primarySpellReleased = false;
    bool secondarySpellReleased = false;
};


#endif
