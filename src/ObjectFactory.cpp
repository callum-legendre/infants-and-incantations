#include "ObjectFactory.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreBullet.h"

// all of the component and script includes were moved to the header file to save some space

#include <memory>

namespace ObjectFactory
{
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
    
