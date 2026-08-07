#ifndef LEVELSCENE_H
#define LEVELSCENE_H

namespace Ogre
{
    class SceneManager;
    class SceneNode;
    class Entity;
}

class LevelScene
{
public:
    // constructor
    explicit LevelScene(Ogre::SceneManager& scnMgr, Ogre::SceneNode& cameraNode);
    ~LevelScene();

    // load and unload functions initialise and shutdown the level
    void load();
    void unload();

private:
    // proporties of the level passed from Engine
    Ogre::SceneManager& scnMgr;
    Ogre::SceneNode& cameraNode;

    // non-passed proporties of the level
    Ogre::SceneNode* rootNode = nullptr;
    Ogre::SceneNode* terrainNode = nullptr;
    
    // methods
    void createTerrain();
    void createLighting();
    void positionCamera();
};


#endif
