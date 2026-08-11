#include "GameObject.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

GameObject::GameObject(Ogre::SceneManager& scnMgr, Ogre::SceneNode* sceneNode)
    : scnMgr(scnMgr), sceneNode(sceneNode)
{
}

GameObject::~GameObject()
{
    if (!sceneNode)
    {
        return;
    }

    // Destroying the node also releases any OgreBullet body stored in its
    // user bindings, while the owning DynamicsWorld is still alive.
    sceneNode->destroyAllChildrenAndObjects();
    scnMgr.destroySceneNode(sceneNode);
    sceneNode = nullptr;
}
