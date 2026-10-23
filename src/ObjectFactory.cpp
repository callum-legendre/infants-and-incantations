#include "ObjectFactory.h"
#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreBullet.h"
#include "OgreUserObjectBindings.h"

// all of the component and script includes were moved to the header file to save some space

#include <memory>

namespace ObjectFactory
{
    // -- create functions -- 
        Ogre::SceneNode* CreatePlayerCamera(Ogre::SceneNode* parent, Ogre::SceneNode* player)
        {
            Ogre::SceneNode* cameraNode = parent->createChildSceneNode("PlayerCameraNode");

            // move camera node to correct starting position
            cameraNode->setPosition(0, 0, 15);
            cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
        
            // create the camera
            Ogre::Camera* cam = cameraNode->getCreator()->createCamera("playerCamera");
            cam->setNearClipDistance(5);
            cam->setAutoAspectRatio(true);
            cameraNode->attachObject(cam);

            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(cameraNode);
            Ogre::UserObjectBindings bindings = cameraNode->getUserObjectBindings();
            bindings.setUserAny("ComponentManager", manager);

            manager->AddComponent<PlayerCameraScript>(std::make_unique<PlayerCameraScript>(manager, player));

            return cameraNode;
        }

    // -- configure functions --
        void ConfigurePlayer(Ogre::SceneNode* playerNode, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            Ogre::Entity* playerMesh = dynamic_cast<Ogre::Entity*>(playerNode->getAttachedObject(0)); // the first attached object is guaranteed to be the entity (for boxes, at least)

            float playerMass = 10.0f;
            btRigidBody* rb = worldPhysics.addRigidBody(playerMass, playerMesh, Ogre::Bullet::CT_CAPSULE);
            rb->setAngularFactor(btVector3(0, 0, 0)); // prevent the capsule from topping which makes the baby roll

            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(playerNode);
            Ogre::UserObjectBindings bindings = playerNode->getUserObjectBindings();
            bindings.setUserAny("ComponentManager", manager);

            manager->AddComponent<InputComponent>(std::make_unique<InputComponent>(manager));
            manager->AddComponent<PhysicsComponent>(std::make_unique<PhysicsComponent>(manager, rb));
            manager->AddComponent<PlayerScript>(std::make_unique<PlayerScript>(manager));
        }
    
        void ConfigureBox(Ogre::SceneNode* boxNode, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(boxNode);
            Ogre::UserObjectBindings bindings = boxNode->getUserObjectBindings();
            bindings.setUserAny("ComponentManager", manager);

            float boxMass = 1.0f;
            Ogre::Entity* boxMesh = dynamic_cast<Ogre::Entity*>(boxNode->getAttachedObject(0)); // the first attached object is guaranteed to be the entity (for boxes, at least)
            btRigidBody* rb = worldPhysics.addRigidBody(boxMass, boxMesh, Ogre::Bullet::CT_BOX);

            manager->AddComponent<PhysicsComponent>(std::make_unique<PhysicsComponent>(manager, rb));
        }

    namespace // namespace is blank to act as pseudo private functions for the factories
    {
        
    }

}
