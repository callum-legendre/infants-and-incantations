#include "PlayerCamera.h"
#include <OgreVector.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

PlayerCamera::PlayerCamera(Ogre::SceneManager& scnMgr) : GameObject(scnMgr, scnMgr.getRootSceneNode()->createChildSceneNode()), playerNode(scnMgr.getSceneNode("PlayerNode"))
{ // create the scene node and pass it to the GameObject constructor, also get the player node and store it in the playerNode property

    // move camera node to correct starting position
    sceneNode->setPosition(0, 0, 15);
    sceneNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
 
    // create the camera
    Ogre::Camera* cam = scnMgr.createCamera("PlayerCamera");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    sceneNode->attachObject(cam);
}

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
