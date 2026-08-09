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
    // inherited methods
    Player(Ogre::SceneNode* playerNode) : GameObject(playerNode)  {}; // constructor

    // overriden methods
    void update(float deltaTime) override; // update function

    private:

    // methods
    InputState getInput();
    void movePlayer(float deltaTime);

    // properties
    static constexpr float moveSpeed = 5.0f; // the multiplier for how fast the player moves (by default)
};

#endif
