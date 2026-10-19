#ifndef SCENENODECOMPONENT_H
#define SCENENODECOMPONENT_H

/*
This file contains both the base classes of the components and the container object that will actually be bound to the the scenenode
The container object will be the function to actually store each component, and will contain member functions for this end.
This is the class that will store the ogre scene node in each object.
Its implimentation is pretty basic and only really needs a constructor and a get method because its just a holder of the node pointer for the most paty
*/

#include "Component.h"

namespace Ogre
{
    class SceneNode;
}

class SceneNodeComponent : public Component
{
    public:
    SceneNodeComponent(Ogre::SceneNode& sceneNode) : mSceneNode(sceneNode) {} // constructor
    ~SceneNodeComponent() = default; // destructor, as it does not own the scene node it shouldnt destroy it accidentally (not a unique_ptr)

    // get method
    Ogre::SceneNode& getSceneNode() {return mSceneNode;}

    private:

    // reference to the scene node
    Ogre::SceneNode& mSceneNode;
};

#endif
