#include "Engine.h"

Engine::Engine()   
{
    // make resource path
    resourcePath = getResourceDirectory();
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Initialise()
{
    // initialise SDL
    SDLInitialise();    

    // initialise the root
    root = std::make_unique<Ogre::Root>((resourcePath / "plugins.cfg").string(), (resourcePath / "ogre.cfg").string(), (resourcePath / "Ogre.log").string());

    // get the GL3+ render system
    Ogre::RenderSystem* rs = root->getRenderSystemByName("OpenGL 3+ Rendering Subsystem");

    if (!rs){
        throw std::runtime_error("Couldn't find GL3+ renderer");
    }

    root->setRenderSystem(rs);

    rs->setConfigOption("Video Mode", "1280 x 720");

    // initialise root, do not create a window
    root->initialise(false);

    // debug
    std::cout << "DEBUG: " << root->getRenderSystem()->getName() << std::endl;
    // end debug

    createOgreWindow();

    // create scene manager
    scnMgr = root->createSceneManager();

    RTSSInitialise();

    resourceInitialise();

    prepareMaterials();

    viewportInitialise();

    loadLevel();

    return true;
}

void Engine::loadLevel()
{
    // instanciate the level
    level = std::make_unique<LevelScene>(*scnMgr, *cameraNode);
    
    // load the level
    level->load();
}

void Engine::SDLInitialise()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(SDL_GetError());
    }   

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    mWindow = SDL_CreateWindow("Infants and Incantations", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!mWindow) {
        throw std::runtime_error(SDL_GetError());
    }
}

void Engine::Run()
{
    bool running = true;

    while (running)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    
                    std::cout << "SIZE_CHANGED\n";

                    int w, h;
                    SDL_GetWindowSize(mWindow, &w, &h);

                    mRenderWindow->resize(w, h);
                    mRenderWindow->windowMovedOrResized();
                }
                break;
            }
        }

        if (mRenderWindow->isClosed()){
            running = false;
            break;
        }

        SDL_PumpEvents();
        if (!root->renderOneFrame()) {
            running = false;
        }
    }
}

void Engine::viewportInitialise()
{
    // also need to tell where we are
    cameraNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cameraNode->setPosition(0, 0, 15);
    cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
 
    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("PlayerCamera");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    cameraNode->attachObject(cam);
 
    // and tell it to render into the main window
    vp = mRenderWindow->addViewport(cam);
    vp->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
    vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME); // tell viewport to render with RTSS techniques instead of default
}

void Engine::Shutdown()
{
    // reset the render window pointer
    mRenderWindow = nullptr;

    // unload the shader genertor
    if (shadergen)
    {
        if (scnMgr)
            shadergen->removeSceneManager(scnMgr);

        Ogre::RTShader::ShaderGenerator::destroy();
        shadergen = nullptr;
    }

    // unload the active level
    level->unload();

    // null out the screen manager
    scnMgr = nullptr;

    // destroy the root
    root.reset();

    // destroy the window
    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    // close SDL
    SDL_Quit();
}

void Engine::resourceInitialise()
{
    //debug
    std::cout << "DEBUG Current working directory: " << std::filesystem::current_path() << '\n';
    // end debug

    // get config file
    Ogre::ConfigFile cf;

    // load resoirces
    cf.load((resourcePath / "resources.cfg").string());

    Ogre::String secName, typeName, archName;

    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();

        auto* settings = seci.getNext();

        for (auto& setting : *settings) {
            typeName = setting.first;
            archName = setting.second;
            
            std::filesystem::path p = archName;

            if (p.is_relative()){
                p = resourcePath / p;
            }
            
            std::cout << "Registering " << p << '\n';

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                p.string(),
                typeName,
                secName);
                    }
    }

    // initialise resource groups
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Engine::RTSSInitialise()
{
    // register our scene with the RTSS
    if (!Ogre::RTShader::ShaderGenerator::initialize()) { // initialise shader generator
        throw std::runtime_error("Couldn't initialise RTShader"); // if failed then throw exception
    }

    shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr(); // get pointer to it

    shadergen->setTargetLanguage("glsl"); // set language for it

    auto cachePath = std::filesystem::temp_directory_path() / "iai_shader_cache"; // create path to temp shader cache directory 
    std::filesystem::create_directories(cachePath); // create the cache
    shadergen->setShaderCachePath(cachePath.string()); // set the gen's cache path

    // add shadergen to the scenemanager
    shadergen->addSceneManager(scnMgr);
}

void Engine::prepareMaterials()
{
    auto materials =
        Ogre::MaterialManager::getSingleton().getResourceIterator();

    while (materials.hasMoreElements())
    {
        auto material =
            Ogre::static_pointer_cast<Ogre::Material>(materials.getNext());

        // Material scripts are parsed during resource-group initialisation,
        // but their supported techniques are not compiled until the material
        // is loaded. RTSS needs that compiled source technique.
        material->load();

        shadergen->createShaderBasedTechnique(
            *material,
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

        shadergen->validateMaterial(
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            *material);
    }
}
