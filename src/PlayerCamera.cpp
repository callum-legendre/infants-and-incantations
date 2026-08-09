#include "PlayerCamera.h"

void PlayerCamera::update(float deltaTime)
{
    // get the player's position in world coordinates
    Ogre::Vector3 playerPos = playerNode->_getDerivedPosition();

    // get target position of the camera
    Ogre::Vector3 targetPos = playerPos + Ogre::Vector3(0.0f, 8.0f, 12.0f);

    // blend value
    constexpr float followSharpness = 8.0f;
    const float blend = 1.0f - std::exp(-followSharpness * deltaTime);

    // move the camera
    sceneNode->setPosition(sceneNode->getPosition() + (targetPos - sceneNode->getPosition()) * blend);

    // point camera at the player
    sceneNode->lookAt(playerPos + Ogre::Vector3(0.0f, 1.0f, 0.0f), Ogre::Node::TS_WORLD);
}
