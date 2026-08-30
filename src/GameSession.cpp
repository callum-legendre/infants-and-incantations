#include "GameSession.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreBullet.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>

#include <algorithm>

#include "PushSpell.h"

GameSession::GameSession(Ogre::SceneManager& scnMgr) : worldPhysics(std::make_unique<Ogre::Bullet::DynamicsWorld>(Ogre::Vector3(0.0f, -9.81f, 0.0f))), scnMgr(scnMgr)
{ // pass in the scene manager and create the dynamics world object for Bullet
    player = std::make_unique<Player>(scnMgr, *worldPhysics);
    playerCamera = std::make_unique<PlayerCamera>(scnMgr);
    LoadLevel();

    // bind the push spell as they player's primary spell (just for testing purposes atm)
    player->SetPrimarySpell(std::make_unique<PushSpell>(scnMgr));
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

    // pass the input state into the player object and run the update function
    player->SetInput(input);
    player->update(deltaTime);

    // update other objects in the level
    currentScene->update(deltaTime);

    // step one frame in the bullet physics simulation
    worldPhysics->getBtWorld()->stepSimulation(deltaTime, 8, 1.0f / 60.0f);

    // update the camera
    playerCamera->update(deltaTime);

    // clean the input struct
    input.primarySpellPressed = false;
    input.primarySpellReleased = false;
    input.secondarySpellPressed = false;
    input.secondarySpellReleased = false;
}

void GameSession::HandleEvent(const SDL_Event& e)
{
    // if the event was mouse button being pressed then assign the proper value to the input struct
    if (e.type == SDL_MOUSEBUTTONDOWN){
        if (e.button.button == SDL_BUTTON_LEFT){
            input.primarySpellPressed = true;
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) {
            input.secondarySpellPressed = true;
        }
    }
    // do the same for the button being released
    else if (e.type == SDL_MOUSEBUTTONUP){
        if (e.button.button == SDL_BUTTON_LEFT){
            input.primarySpellReleased = true;
        }
        else if (e.button.button == SDL_BUTTON_RIGHT) {
            input.secondarySpellReleased = true;
        }
    }
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

    // create float pointers to store the mouse position
    int xPos = 0;
    int yPos = 0;

    // assign mouse popsition
    const Uint32 mouse = SDL_GetMouseState(&xPos, &yPos);

    // fetch the viewport attached to the camera
    Ogre::Viewport* viewport = playerCamera->getViewport();

    // some null and zero checking for the viewport, just in case
    if (!viewport || viewport->getActualWidth() <= 0 || viewport->getActualHeight() <= 0) {return;}

    // get the renderwindow
    auto* renderWindow = dynamic_cast<Ogre::RenderWindow*>(viewport->getTarget());

    // get the point to pixel scale from the render window
    const float pointToPixelScale = renderWindow ? renderWindow->getViewPointToPixelScale() : 1.0f;

    // find the pixel that the mouse is currently located in
    const float mousePixelX = static_cast<float>(xPos) * pointToPixelScale;
    const float mousePixelY = static_cast<float>(yPos) * pointToPixelScale;

    // convert x and y coordindates and assign them to the input struct
    input.mLocationX = std::clamp((mousePixelX - static_cast<float>(viewport->getActualLeft())) / static_cast<float>(viewport->getActualWidth()), 0.0f, 1.0f);
    input.mLocationY = std::clamp((mousePixelY - static_cast<float>(viewport->getActualTop())) / static_cast<float>(viewport->getActualHeight()), 0.0f, 1.0f);
}

Ogre::Camera* GameSession::GetCamera()
{
    return static_cast<Ogre::Camera*>(playerCamera->GetSceneNode()->getAttachedObject("PlayerCamera"));
}

GameSession::~GameSession() = default;
