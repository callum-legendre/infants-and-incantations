#include "ObjectFactory.h"
#include "ObjectNames.h"

#include "OgreSceneNode.h"
#include "OgreEntity.h"
#include "OgreBullet.h"
#include "OgreUserObjectBindings.h"

// all of the component and script includes were moved to the header file to save some space

#include <memory>
#include <string>

namespace ObjectFactory
{
    namespace // namespace is blank to act as pseudo private functions for the factories
    {
        void ConfigurePlayer(Ogre::SceneNode* playerNode, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            Ogre::Entity* playerMesh = dynamic_cast<Ogre::Entity*>(playerNode->getAttachedObject(0)); // the first attached object is guaranteed to be the entity (for boxes, at least)

            float playerMass = 10.0f;
            btRigidBody* rb = worldPhysics.addRigidBody(playerMass, playerMesh, Ogre::Bullet::CT_CAPSULE);
            rb->setAngularFactor(btVector3(0, 0, 0)); // prevent the capsule from topping which makes the baby roll

            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(playerNode);
            Ogre::UserObjectBindings bindings = playerNode->getUserObjectBindings();
            bindings.setUserAny("ComponentManager", manager);

            manager->AddComponent<InputComponent>(*manager);
            manager->AddComponent<PhysicsComponent>(*manager, *rb);
            manager->AddComponent<PlayerScript>(*manager);
        }
    
        void ConfigureBox(Ogre::SceneNode* boxNode, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(boxNode);
            Ogre::UserObjectBindings bindings = boxNode->getUserObjectBindings();
            bindings.setUserAny("ComponentManager", manager);

            float boxMass = 5.0f;
            Ogre::Entity* boxMesh = dynamic_cast<Ogre::Entity*>(boxNode->getAttachedObject(0)); // the first attached object is guaranteed to be the entity (for boxes, at least)
            btRigidBody* rb = worldPhysics.addRigidBody(boxMass, boxMesh, Ogre::Bullet::CT_BOX);

            manager->AddComponent<PhysicsComponent>(*manager, *rb);
        }

        void ConfigurePlane(Ogre::SceneNode* planeNode, Ogre::Bullet::DynamicsWorld& worldPhysics)
        {
            std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(planeNode);
            Ogre::UserObjectBindings bindings = planeNode->getUserObjectBindings();
            bindings.setUserAny("ComponentManager", manager);

            Ogre::Entity* planeMesh = dynamic_cast<Ogre::Entity*>(planeNode->getAttachedObject(0)); // the first attached object is guaranteed to be the entity (for boxes, at least)
            btRigidBody* rb = worldPhysics.addRigidBody(0.0f, planeMesh, Ogre::Bullet::CT_TRIMESH); // mass 0 because it should be immovable

            manager->AddComponent<PhysicsComponent>(*manager, *rb);
        }
    }

    void ConfigureObjectComponents(std::string objectType, Ogre::Node* node, Ogre::Bullet::DynamicsWorld& worldPhysics)
    {
        // convert to sceneNode
        Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(node);

        if (objectType == ObjectNames::Player){
            ConfigurePlayer(sceneNode, worldPhysics);
        }
        else if (objectType == ObjectNames::Box){
            ConfigureBox(sceneNode, worldPhysics);
        }
        else if (objectType == ObjectNames::Plane){
            ConfigurePlane(sceneNode, worldPhysics);
        }
    }

    // -- create functions -- 
    Ogre::SceneNode* CreatePlayerCamera(Ogre::SceneNode* parent, Ogre::SceneNode* player)
    {
        Ogre::SceneNode* cameraNode = parent->createChildSceneNode(ObjectNames::PlayerCamera);

        // move camera node to correct starting position
        cameraNode->setPosition(0, 0, 15);
        cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
    
        // create the camera
        Ogre::Camera* cam = cameraNode->getCreator()->createCamera(ObjectNames::PlayerCamera);
        cam->setNearClipDistance(5);
        cam->setAutoAspectRatio(true);
        cameraNode->attachObject(cam);

        std::shared_ptr<ComponentManager> manager = std::make_shared<ComponentManager>(cameraNode);
        Ogre::UserObjectBindings bindings = cameraNode->getUserObjectBindings();
        bindings.setUserAny("ComponentManager", manager);

        manager->AddComponent<PlayerCameraScript>(*manager, player);

        return cameraNode;
    }
}


