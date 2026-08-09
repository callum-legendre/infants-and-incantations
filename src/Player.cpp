#include "Player.h"

#include <OgreVector.h>

void Player::OnUpdate(float deltaTime)
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
    if (input.forward) {direction.z += 1.0f;}
    if (input.back) {direction.z -= 1.0f;}
    if (input.left) {direction.x -= 1.0f;}
    if (input.right) {direction.x += 1.0f;}

    // move the player object
    if (direction.isZeroLength() == false){ // only move if the player presses movement input

        // normalise direction
        direction.normalise();

        // update the player's transform
        transform.Translate(direction * deltaTime * moveSpeed);

        // turn model in direction of movement
        Ogre::Quaternion targetRotation = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
        transform.SetRotation(targetRotation);
    }
}
