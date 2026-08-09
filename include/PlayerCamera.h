#ifndef PLAYERCAMERA_H
#define PLAYERCAMERA_H
/*
Defines the GameObject that contains the camera which follows the player.
*/

#include "GameObject.h"

namespace Ogre
{
    class SceneNode;
}

class PlayerCamera : public GameObject
{
    public:
    // inherted methods
    PlayerCamera(Ogre::SceneNode* cameraNode, Ogre::SceneNode* playerNode) : GameObject(cameraNode), playerNode(playerNode) {} // cpmstructor

    // overriden methods
    void update(float deltaTime) override;

    private:
    // properties
    Ogre::SceneNode* playerNode = nullptr; // stores a reference to the player node for camera positioning relative to the player

};

#endif