#include "GameSession.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreBullet.h>

GameSession::GameSession(Ogre::SceneManager& scnMgr) : worldPhysics(std::make_unique<Ogre::Bullet::DynamicsWorld>(Ogre::Vector3(0.0f, -9.81f, 0.0f))), scnMgr(scnMgr)
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
    // get the player's inputs
    GetInput();

    // update the player
    player->update(deltaTime, input);

    // update other objects in the level
    currentScene->update(deltaTime);

    // step one frame in the bullet physics simulation
    worldPhysics->getBtWorld()->stepSimulation(deltaTime, 8, 1.0f / 60.0f);

    // update the camera
    playerCamera->update(deltaTime);
}

void GameSession::GetInput()
{
    // get the input state
    const Uint8* keys = SDL_GetKeyboardState(nullptr);

    // assign the input state to the input struct property
    input.forward = static_cast<bool>(keys[SDL_SCANCODE_W]);
    input.left = static_cast<bool>(keys[SDL_SCANCODE_A]);
    input.back = static_cast<bool>(keys[SDL_SCANCODE_S]);
    input.right = static_cast<bool>(keys[SDL_SCANCODE_D]);
    input.mLocationX = 0.0;
    input.mLocationY = 0.0;

    // assign mouse popsition
    const Uint32* mouse = SDL_GetMouseState(*input.mLocationX, *input.mLocationY);
}

Ogre::Camera* GameSession::GetCamera()
{
    return static_cast<Ogre::Camera*>(playerCamera->GetSceneNode()->getAttachedObject("PlayerCamera"));
}

GameSession::~GameSession() = default;
