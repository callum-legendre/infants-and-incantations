#include "scripts/PlayerCameraScript.h"

#include "ComponentManager.h"

#include "OgreVector.h"
#include "OgreSceneNode.h"
#include "OgreSceneManager.h"

PlayerCameraScript::PlayerCameraScript(ComponentManager& parent, Ogre::SceneNode* player) : ScriptComponent(parent), mPlayerReference(player) {}

void PlayerCameraScript::Update(float deltaTime)
{
    // get the player node
    Ogre::SceneNode* cameraNode = mParent.GetParentNode();

    // get the player's position in world coordinates
    Ogre::Vector3 playerPos = mPlayerReference->_getDerivedPosition();
    // get target position of the camera
    Ogre::Vector3 targetPos = playerPos + Ogre::Vector3(0.0f, 8.0f, 12.0f);

    // blend value
    const float blend = 1.0f - std::exp(-mFollowSharpness * deltaTime);

    // move the camera
    cameraNode->setPosition(cameraNode->getPosition() + (targetPos - cameraNode->getPosition()) * blend);

    // point camera at the player
    cameraNode->lookAt(playerPos + Ogre::Vector3(0.0f, 1.0f, 0.0f), Ogre::Node::TS_WORLD);
}