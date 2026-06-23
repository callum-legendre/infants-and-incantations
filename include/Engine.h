#ifndef ENGINE_H
#define ENGINE_H

// standard module includes
#include <iostream>

// ogre includes
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreLight.h>
#include <OgreApplicationContext.h>
#include <OgreEntity.h>

// declaration of engine
class Engine
{
    public:
    Engine(); // constructor
    bool Initialise(); // iniitalises the engine and opens window
    void Run(); // updates each frame of the window

    private:
    OgreBites::ApplicationContext ctx; // application context
    Ogre::Root* root; // the root object needed for all ogre things
    Ogre::SceneManager* scnMgr; // scene manager
    Ogre::RTShader::ShaderGenerator* shadergen; // shader generator


    // Ogre::RenderWindow* window; // the render window object that the window is 

};

#endif
