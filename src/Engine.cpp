#include "engine.h"

Engine::Engine()
{
    /*
    // get and then set the avaliable render system
    auto renderers = root.getAvailableRenderers();

    // check if renderers empty
    if (renderers.empty()){
        std::cout << "no renderers found" << std::endl;
    }

    // otherwise set the render system to the first one
    root.setRenderSystem(renderers.front());

    // create scene manager
    scnMgr = root.createSceneManager();
    */

    // instanciate context and initialise app
    OgreBites::ApplicationContext ctx("Infants and Incantations");
    ctx.initApp();

    // get pointer to the root
    Ogre::Root* root = ctx.getRoot();

    // get pointer to the scene manager
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // get shader generator and set it to singleton
    shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr); // add the scene manager to the shader generator (register it with generator)

    // get some light in here
    Ogre::Light* light = scnMgr->createLight("MainLight"); // creates a light in scene manager and directs pointer to it
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode(); // creates a node in the scene for the light
    lightNode->attachObject(light); // attach the light to the newly created node
    lightNode->setPosition(20, 80, 50); // set the position of the light

    // create a camera
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode(); // creates a node in the scene
    Ogre::Camera* cam = scnMgr->createCamera("myCam"); // creates a camera obejct in the scene

    // set proporties of the camera
    cam->setNearClipDistance(5); 
    cam->setAutoAspectRatio(true);

    camNode->attachObject(cam); // attach the camera to the newly created node
    camNode->setPosition(0, 0, 140); // set the position of the camera 

    // render the camera view into the main window
    ctx.getRenderWindow()->addViewport(cam);

    // create an entity to render
    // Ogre::Entity* ogreEntity = scnMgr->createEntity("ogrehead.mesh"); (commented becuase currenlty this file doesnt exist and crashes the program)
    Ogre::SceneNode* ogreNode = scnMgr->getRootSceneNode()->createChildSceneNode(); // create another scene node
    ogreNode->attachObject(ogreEntity); // attach the ogre to the node
}

bool Engine::Initialise()
{
    // initialise ogre without creating the window
    // root.initialise(false);



    // create the window now
    // window = root.createRenderWindow("Test Window", 1280, 720, false);

    return true;
}

void Engine::Run()
{
    // run updates for as long as the window is open
    // if (window->isClosed()){

        // root.startRendering();
    // }
}