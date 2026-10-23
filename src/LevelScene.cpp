#include "LevelScene.h" // header file

#include "ObjectFactory.h"
#include "ComponentManager.h"
#include "ObjectNames.h"

#include <string>
#include <memory>

// ogre includes
#include "OgreSceneManager.h"
#include "OgreLight.h"
#include "OgreBullet.h"
#include "OgreSceneNode.h"
#include "OgreUserObjectBindings.h"
#include "OgreEntity.h"

LevelScene::LevelScene(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics) : scnMgr(scnMgr), worldPhysics(worldPhysics)
{} // takes screen manager and the game dynamic as an argument

void LevelScene::Load()
{
    // create pseudo root node for the level
    mRootNode = scnMgr.getRootSceneNode()->createChildSceneNode("LevelRoot");

    // load the DotScene
    mRootNode->loadChildren("testScene.scene");

    ObjectFactory::CreatePlayerCamera(mRootNode, scnMgr.getSceneNode(ObjectNames::Player));

    CreateComponents(); // create the colliders and attach them to all objects that need it
    CreateLighting(); // TEMPORARY AS MY TEST SCENE DOES NOT HAVE ANY LIGHTING BY DEFAULT
}

void LevelScene::CreateLighting()
{
    // create light
    Ogre::Light* light = scnMgr.createLight("LevelMainLight");

    // create the light node
    Ogre::SceneNode* lightNode = mRootNode->createChildSceneNode();

    // set position of the light
    lightNode->setPosition(0, 10, 15);

    // attach light to the node
    lightNode->attachObject(light);
}

void LevelScene::CreateComponents(Ogre::Node* node)
{
    if (node == nullptr){
        node = mRootNode;
    }

    // iterate through all attached objects of the node that was passed in
    for (Ogre::Node* child : node->getChildren())
    {
        Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*>(child);

        for (Ogre::MovableObject* object : sceneNode->getAttachedObjects()){

            Ogre::Entity* entity = dynamic_cast<Ogre::Entity*>(object);

            if (!entity) continue;

            const auto& value = entity->getUserObjectBindings().getUserAny("objectType");

            // if there is no value then skip
            if (!value.has_value()) continue;

            // get the string value of the object binding
            std::string objectType = Ogre::any_cast<std::string>(value);

            ObjectFactory::ConfigureObjectComponents(objectType, child, worldPhysics);

            break;
        }

        CreateComponents(child);
    }
}

void LevelScene::Unload()
{
    // null check on the root node just in case
    if (!mRootNode){
        return;
    }

    // destroy everything else in the scene
    mRootNode->destroyAllChildrenAndObjects();

    // destroy the pseudo root
    scnMgr.destroySceneNode(mRootNode);

    // set relevant pointers to null
    mRootNode = nullptr;

    // destroy the plane mesh
    auto& meshManager = Ogre::MeshManager::getSingleton();
}

LevelScene::~LevelScene()
{
    Unload();
}

void LevelScene::Input(Ogre::Node* parent)
{
    if (parent == nullptr){
        parent = mRootNode;
    }

    // iterate through each node
    for (Ogre::Node* child : parent->getChildren()){

        Ogre::Any managerAny = child->getUserObjectBindings().getUserAny("ComponentManager");
        if (!managerAny.isEmpty()){

            std::shared_ptr<ComponentManager> manager = Ogre::any_cast<std::shared_ptr<ComponentManager>>(managerAny);

            InputComponent* inputComponent = manager->GetComponent<InputComponent>();

            if (inputComponent){
                inputComponent->OnInput();
            }
        }

        Input(child);
    }
}

void LevelScene::Update(float deltaTime, Ogre::Node* parent)
{
    if (parent == nullptr){
        parent = mRootNode;
    }

    // iterate through each node for the scripts
    for (Ogre::Node* child : parent->getChildren()){

        Ogre::Any managerAny = child->getUserObjectBindings().getUserAny("ComponentManager");
        if (!managerAny.isEmpty()){

            std::shared_ptr<ComponentManager> manager = Ogre::any_cast<std::shared_ptr<ComponentManager>>(managerAny);

            manager->UpdateScripts(deltaTime);
        }

        Update(deltaTime, child);
    }
}
