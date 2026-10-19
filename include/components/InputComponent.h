#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
/*
I wanted to have a component that handles the player's keyboard and mouse input.
This potentially allows for more interesting stuff of being able to mirror the player's input in some place or do other fun stuff with it outside of being hardcoded to the player character
*/

#include "components/Component.h"

struct InputState // for storing the input state 
{
    bool forward = false;
    bool left = false;
    bool back = false;
    bool right = false;
};

class InputComponent : Component
{
    public:

    InputComponent(ComponentManager& parent) : Component(parent) {}

    void OnInput();

    InputState GetInput();

    private:

    InputState mInput;
};




#endif
