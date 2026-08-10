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
#include "GameSession.h"

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

    // other objects
    std::unique_ptr<GameSession> session;

    void SDLInitialise();
    void createOgreWindow();
    void viewportInitialise();
    void resourceInitialise();
    void prepareMaterials();
    std::filesystem::path getResourceDirectory() const;
    void RTSSInitialise();
};

#endif
