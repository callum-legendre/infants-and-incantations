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
    bool lClickPressed = false;
    bool lClickReleased = false;
    bool rClickPressed = false;
    bool rClickReleased = false;
};


#endif
