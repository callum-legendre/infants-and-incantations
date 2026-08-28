#ifndef GAMESESSION_H
#define GAMESESSION_H

/*
The class that handles the actual game session. This is seperated from engine to create a seperation between code necissary to set up the game, and actual game logic.
*/

#include "Player.h"
#include "PlayerCamera.h"
#include "LevelScene.h"
#include "InputState.h"

#include <SDL2/SDL.h>

namespace Ogre
{
    class SceneManager;
    class Camera;
    
    namespace Bullet
    {
        class DynamicsWorld;
    }
}

class GameSession
{
    public:
    GameSession(Ogre::SceneManager& scnMgr); // constructor
    void update(float deltaTime); // update function
    Ogre::Camera* GetCamera(); // method that gets the camera, used in engine to assign the camera to the viewport
    void HandleEvent(const SDL_Event& e); // method that handles all the other SDL events and assigns all input relevant ones
    ~GameSession(); // destructor

    private:
    // properties
    std::unique_ptr<Ogre::Bullet::DynamicsWorld> worldPhysics = nullptr;
    std::unique_ptr<Player> player = nullptr;
    std::unique_ptr<PlayerCamera> playerCamera = nullptr;
    std::unique_ptr<LevelScene> currentScene = nullptr;
    Ogre::SceneManager& scnMgr;
    InputState input;

    // methods
    void LoadLevel(); // loads the level
    void GetInput(); // method that gets the p[layer's keybaord input and mouse location each frame
};


#endif
