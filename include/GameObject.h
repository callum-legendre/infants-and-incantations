/*
This is the base class for all obejcts that will exist in the game, meaning every object will inherit from this class.
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <Ogre.h> // include Ogre


class GameObject
{
    public:
    explicit GameObject(Ogre::SceneNode* sceneNode) : sceneNode(sceneNode) // constructor
    {

    }

    virtual void update(float deltaTime) // update that runs every frame, is overriden in all necissary objects
    {
        
    }
    virtual ~GameObject() = default; // destructor

    protected: // proporties + methods that can be accessed by child classes
    Ogre::SceneNode* sceneNode = nullptr;

    private:
}

#endif
