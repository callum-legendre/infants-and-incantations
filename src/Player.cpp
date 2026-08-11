#include "Player.h"

#include <OgreVector.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>

Player::Player(Ogre::SceneManager& scnMgr) : GameObject(scnMgr.getRootSceneNode()->createChildSceneNode("PlayerNode"))
{ // creates the player node and passes it to the game object constructor before doing anything else

    Ogre::Entity* playerMesh = scnMgr.createEntity("baby.mesh"); // create entity mesh

    sceneNode->attachObject(playerMesh); // attach the mesh to the node
}

void Player::update(float deltaTime)
{
    movePlayer(deltaTime);
}

InputState Player::getInput()
{
    // get the input state
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    InputState inputState {
        .forward = static_cast<bool>(keys[SDL_SCANCODE_W]),
        .left = static_cast<bool>(keys[SDL_SCANCODE_A]),
        .back = static_cast<bool>(keys[SDL_SCANCODE_S]),
        .right = static_cast<bool>(keys[SDL_SCANCODE_D])
    };

    return inputState;
}

void Player::movePlayer(float deltaTime)
{
    InputState input = getInput(); // get input keys

    Ogre::Vector3 direction = Ogre::Vector3::ZERO; // create movement vector

    // apply movement from input
    if (input.forward) {direction.z -= 1.0f;}
    if (input.back) {direction.z += 1.0f;}
    if (input.left) {direction.x -= 1.0f;}
    if (input.right) {direction.x += 1.0f;}

    // move the player object
    if (direction.isZeroLength() == false){ // only move if the player presses movement input

        // normalise direction
        direction.normalise();

        // update the player's location
        GetSceneNode()->translate(direction * deltaTime * moveSpeed);

        // turn model in direction of movement
        Ogre::Quaternion targetRotation = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
        GetSceneNode()->setOrientation(targetRotation);
    }
}

