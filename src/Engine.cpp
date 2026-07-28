#include "engine.h"

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

    sceneInitialise();

    return true;
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

void Engine::sceneInitialise()
{
    // without light we would just get a black screen    
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 10, 15);
    lightNode->attachObject(light);
 
    // also need to tell where we are
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 15);
    camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
 
    // create the camera
    cam = scnMgr->createCamera("myCam");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
 
    // and tell it to render into the main window
    vp = mRenderWindow->addViewport(cam);
    vp->setBackgroundColour(Ogre::ColourValue(1.0f, 0.0f, 1.0f));
    vp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME); // tell viewport to render with RTSS techniques instead of default

    // finally something to render
    Ogre::Entity* ent = scnMgr->createEntity("Sinbad.mesh");

    // Creating the entity loads the materials referenced by the mesh. Generate
    // their GL3+ shader techniques only after that has happened.
    for (unsigned int i = 0; i < ent->getNumSubEntities(); ++i)
    {
        auto mat = ent->getSubEntity(i)->getMaterial();

        const bool techniqueCreated = shadergen->createShaderBasedTechnique(
            *mat,
            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

        if (!techniqueCreated)
        {
            throw std::runtime_error(
                "Could not generate an RTSS technique for material '" +
                mat->getName() + "'");
        }

        shadergen->validateMaterial(
            Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
            *mat);
    }

    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
}

void Engine::Shutdown()
{
    mRenderWindow = nullptr;

    if (shadergen)
    {
        if (scnMgr)
            shadergen->removeSceneManager(scnMgr);

        Ogre::RTShader::ShaderGenerator::destroy();
        shadergen = nullptr;
    }

    scnMgr = nullptr;
    root.reset();

    if (mWindow)
    {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }

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
