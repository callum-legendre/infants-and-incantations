#ifndef LEVELSCENE_H
#define LEVELSCENE_H

#include <vector>

namespace Ogre
{
    class SceneManager;
    class SceneNode;
    class Entity;
    class Node;

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
    void Load();
    void Unload();

    void Update(float deltaTime, Ogre::Node* = nullptr);
    void Input(Ogre::Node* parent = nullptr);

private:
    // world properties
    Ogre::SceneManager& scnMgr;
    Ogre::Bullet::DynamicsWorld& worldPhysics;

    // non-passed proporties of the level
    Ogre::SceneNode* mRootNode = nullptr;
    Ogre::SceneNode* mTerrainNode = nullptr;
    
    // methods
    void CreateComponents(Ogre::Node* node = nullptr);
    void CreateLighting();
};


#endif
