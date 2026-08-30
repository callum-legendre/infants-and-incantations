#ifndef INPUTSTATE_H
#define INPUTSTATE_H

struct InputState
{
    bool forward = false;
    bool left = false;
    bool back = false;
    bool right = false;
    float mLocationX = 0.0f;
    float mLocationY = 0.0f;
    bool primarySpellPressed = false;
    bool secondarySpellPressed = false;
    bool primarySpellReleased = false;
    bool secondarySpellReleased = false;
};


#endif
