#include "GameSession.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreBullet.h>

GameSession::GameSession(Ogre::SceneManager& scnMgr) : worldPhysics(std::make_unique<Ogre::Bullet::DynamicsWorld>(Ogre::Vector3(0.0f, -9.81f, 0.0f))), scnMgr(scnMgr)
{ // pass in the scene manager and create the dynamics world object for Bullet
    LoadLevel();
}

void GameSession::LoadLevel()
{
    // unload the previous level if one is loaded
    if (currentScene != nullptr){
        currentScene->Unload();
    }

    // instanciate the level
    currentScene = std::make_unique<LevelScene>(scnMgr, *worldPhysics);
    
    // load the level
    currentScene->Load();
}

void GameSession::Input()
{
    currentScene->Input();
}

void GameSession::Update(float deltaTime)
{
    // update the player and the other objects in the level
    currentScene->Update(deltaTime); 

    // step one frame in the bullet physics simulation
    worldPhysics->getBtWorld()->stepSimulation(deltaTime, 8, 1.0f / 60.0f);
}

GameSession::~GameSession() = default;
