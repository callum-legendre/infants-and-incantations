#include "GameSession.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreBullet.h>

GameSession::GameSession(Ogre::SceneManager& scnMgr) : scnMgr(scnMgr), worldPhysics(std::make_unique<Ogre::Bullet::DynamicsWorld>(Ogre::Vector3(0.0f, -9.81f, 0.0f)))
{ // pass in the scene manager and create the dynamics world object for Bullet
    player = std::make_unique<Player>(scnMgr, *worldPhysics);
    playerCamera = std::make_unique<PlayerCamera>(scnMgr);
    LoadLevel();
}

void GameSession::LoadLevel()
{
    // unload the previous level if one is loaded
    if (currentScene != nullptr){
        currentScene->unload();
    }

    // instanciate the level
    currentScene = std::make_unique<LevelScene>(scnMgr, *worldPhysics);
    
    // load the level
    currentScene->load();
}

void GameSession::update(float deltaTime)
{
    // update the player and the other objects in the level
    player->update(deltaTime);
    currentScene->update(deltaTime);

    // step one frame in the bullet physics simulation
    worldPhysics->getBtWorld()->stepSimulation(deltaTime, 8, 1.0f / 60.0f);

    // update the camera
    playerCamera->update(deltaTime);
}

Ogre::Camera* GameSession::GetCamera()
{
    return static_cast<Ogre::Camera*>(playerCamera->GetSceneNode()->getAttachedObject("PlayerCamera"));
}

GameSession::~GameSession() = default;
