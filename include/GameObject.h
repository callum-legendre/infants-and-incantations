/*
This is the base class for all obejcts that will exist in the game, meaning every object will inherit from this class.
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <Ogre.h> // include Ogre


class GameObject
{
    public:
    GameObject(); // constructor
    void update(float deltaTime) // update that runs every frame
    ~GameObject(); // destructor


    private:
}

#endif
