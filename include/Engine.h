#ifndef ENGINE_H
#define ENGINE_H

// standard module includes
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <algorithm>

// Ogre includes
#include <Ogre.h>
#include <OgreRTShaderSystem.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// other includes from this project
#include "LevelScene.h"
#include "Player.h"
#include "PlayerCamera.h"

// declaration of engine class
class Engine
{
public:
    // constructor 
    Engine();
    ~Engine();

    bool Initialise();

    // render loop
    void Run();

    void Shutdown();

private:
    // ogre objects
    std::unique_ptr<Ogre::Root> root;
    Ogre::SceneManager* scnMgr = nullptr;
    Ogre::RTShader::ShaderGenerator* shadergen = nullptr;
    Ogre::Viewport* vp = nullptr;
    Ogre::RenderWindow* mRenderWindow = nullptr;

    // SDL objects
    SDL_Window* mWindow = nullptr;

    // resource path
    std::filesystem::path resourcePath;

    // active level
    std::unique_ptr<LevelScene> level;

    // player object
    std::unique_ptr<Player> player;

    // player camera object
    std::unique_ptr<PlayerCamera> camera;

    void SDLInitialise();
    void createOgreWindow();
    void viewportInitialise();
    void resourceInitialise();
    void prepareMaterials();
    std::filesystem::path getResourceDirectory() const;
    void RTSSInitialise();
    void loadLevel();
    void createPlayer();
};

#endif
