#include "ObjectFactory.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreBullet.h"

// all of the component and script includes were moved to the header file to save some space

#include <memory>

namespace ObjectFactory
{
    // -- create functions -- 
        Ogre::SceneNode* CreatePlayer(Ogre::SceneNode* parent, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            Ogre::SceneNode* player = parent->createChildSceneNode("player");
            player->setPosition(0.0f, 2.0f, 0.0f); // move the player above the plane so it does not clip through

            Ogre::Entity* playerMesh = player->getCreator()->createEntity("baby.mesh"); 
            player->attachObject(playerMesh);

            float playerMass = 10.0f;
            btRigidBody* rb = worldPhysics.addRigidBody(playerMass, playerMesh, Ogre::Bullet::CT_CAPSULE);
            rb->setAngularFactor(btVector3(0, 0, 0)); // prevent the capsule from topping which makes the baby roll

            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(player);
            player->setUserAny(manager);

            manager->AddComponent<InputComponent>(std::make_unique<InputComponent>(manager));
            manager->AddComponent<PhysicsComponent>(std::make_unique<PhysicsComponent>(manager, rb));
            manager->AddComponent<PlayerScript>(std::make_unique<PlayerScript>(manager));

            return player;
        }
    
    // -- configure functions --
        void ConfigureBox(Ogre::SceneNode* boxNode, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(boxNode);
            boxNode->setUserAny(manager);

            float boxMass = 1.0f;
            Ogre::Entity* boxMesh = dynamic_cast<Ogre::Entity*>(boxNode->getAttachedObject(0)); // the first attached object is guaranteed to be the entity (for boxes, at least)
            btRigidBody* rb = worldPhysics.addRigidBody(boxMass, boxMesh, Ogre::Bullet::CT_BOX);

            manager->AddComponent<PhysicsComponent>(std::make_unique<PhysicsComponent>(manager, rb));
        }

    namespace // namespace is blank to act as pseudo private functions for the factories
    {
        
    }

}
