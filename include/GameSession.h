#ifndef GAMESESSION_H
#define GAMESESSION_H

/*
The class that handles the actual game session. This is seperated from engine to create a seperation between code necissary to set up the game, and actual game logic.
*/

#include "Player.h"
#include "PlayerCamera.h"
#include "LevelScene.h"

namespace Ogre
{
    class SceneManager;
    class Camera;
}

class GameSession
{
    public:
    GameSession(Ogre::SceneManager* scnMgr); // constructor
    void update(float deltaTime); // update function
    Ogre::Camera* GetCamera(); // method that gets the camera, used in engine to assign the camera to the viewport
    ~GameSession() = default; // destructor

    private:
    // properties
    std::unique_ptr<Player> player = nullptr;
    std::unique_ptr<PlayerCamera> playerCamera = nullptr;
    std::unique_ptr<LevelScene> currentScene = nullptr;
    Ogre::SceneManager* scnMgr = nullptr;

    // methods
    void CreatePlayer();
    void CreatePlayerCamera();
    void LoadLevel();
};


#endif
