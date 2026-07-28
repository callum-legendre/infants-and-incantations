#ifndef ENGINE_H
#define ENGINE_H

// standard module includes
#include <filesystem>
#include <iostream>
#include <stdexcept>

// ogre includes
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreConfigFile.h>
#include <OgreResourceGroupManager.h>
#include <OgreRTShaderSystem.h>
#include <OgreStringConverter.h>
#include <OgreMaterialManager.h>

// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// declaration of engine
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
    Ogre::Camera* cam = nullptr;
    Ogre::Viewport* vp = nullptr;
    Ogre::RenderWindow* mRenderWindow = nullptr;

    // SDL objects
    SDL_Window* mWindow = nullptr;

    // resource path
    std::filesystem::path resourcePath;

    void SDLInitialise();
    void createOgreWindow();
    void sceneInitialise();
    void resourceInitialise();
    std::filesystem::path getResourceDirectory() const;
    void RTSSInitialise();
};

#endif
