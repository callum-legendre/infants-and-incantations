#ifndef LEVELSCENE_H
#define LEVELSCENE_H

#include <vector>
#include "GameObject.h"

namespace Ogre
{
    class SceneManager;
    class SceneNode;
    class Entity;

    namespace Bullet
    {
        class DynamicsWorld;
    }
}

class LevelScene
{
public:
    // constructor
    explicit LevelScene(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics);
    ~LevelScene();

    // load and unload functions initialise and shutdown the level
    void load();
    void unload();

    // update function runs every frame
    void update(float deltaTime);

private:
    // world properties
    Ogre::SceneManager& scnMgr;
    Ogre::Bullet::DynamicsWorld& worldPhysics;

    // non-passed proporties of the level
    Ogre::SceneNode* rootNode = nullptr;
    Ogre::SceneNode* terrainNode = nullptr;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    
    // methods
    void createTerrain();
    void createLighting();
};


#endif
