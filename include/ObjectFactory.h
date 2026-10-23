#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

/*
The object factory will be factory functions in a namespace rather than an object
as this allows for more flexibility rather than all of them being in a class.
*/

#include "ComponentManager.h"
#include "components/InputComponent.h"
#include "components/PhysicsComponent.h"

#include "scripts/PlayerScript.h"
#include "scripts/PlayerCameraScript.h"

namespace Ogre
{
    class SceneNode;

    namespace Bullet
    {
        class DynamicsWorld;
    }
}

namespace ObjectFactory
{
    Ogre::SceneNode* CreatePlayerCamera(Ogre::SceneNode* parent, Ogre::SceneNode player);
    void ConfigurePlayer(Ogre::SceneNode* parent, Ogre::Bullet::DynamicsWorld& worldPhysics);
    void ConfigureBox(Ogre::SceneNode* parent, Ogre::Bullet::DynamicsWorld& worldPhysics);
};

#endif
