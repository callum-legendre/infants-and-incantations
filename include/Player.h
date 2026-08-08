#ifndef PLAYER_H
#define PLAYER_H

/*
The player object that the player controls. 
It exists outside of all of the LevelScenes, so it survives loading and unloading of them. 
*/

#include <SDL2/SDL_keyboard.h>

#include "GameObject.h"
#include "InputState.h"

namespace Ogre
{
    class SceneNode;
}

class Player : public GameObject // inherits from GameObject class
{
    public:

    void update(float deltaTime) override; // override method for the update function

    private:

    InputState getInput();

};

#endif
