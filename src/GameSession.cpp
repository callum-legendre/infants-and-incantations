#include "GameSession.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCamera.h>

GameSession::GameSession(Ogre::SceneManager& scnMgr) : scnMgr(scnMgr)
{
    player = std::make_unique<Player>(scnMgr);
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
    currentScene = std::make_unique<LevelScene>(scnMgr);
    
    // load the level
    currentScene->load();
}

void GameSession::update(float deltaTime)
{
    // update the player and the player camera
    player->update(deltaTime);
    playerCamera->update(deltaTime);
    currentScene->update(deltaTime);
}

Ogre::Camera* GameSession::GetCamera()
{
    return static_cast<Ogre::Camera*>(playerCamera->GetSceneNode()->getAttachedObject("PlayerCamera"));
}
