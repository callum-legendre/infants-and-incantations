#ifndef GAMESESSION_H
#define GAMESESSION_H

/*
The class that handles the actual game session. This is seperated from engine to create a seperation between code necissary to set up the game, and actual game logic.
*/

#include "LevelScene.h"

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
    void Input();
    void Update(float deltaTime); // update function
    ~GameSession(); // destructor

    private:
    // properties
    std::unique_ptr<Ogre::Bullet::DynamicsWorld> worldPhysics = nullptr;
    std::unique_ptr<LevelScene> currentScene = nullptr;
    Ogre::SceneManager& scnMgr;

    // methods
    void LoadLevel();
};


#endif
