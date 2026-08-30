#include "LevelScene.h" // header file

// ogre includes
#include <OgreEntity.h>
#include <OgreLight.h>
#include <OgreMeshManager.h>
#include <OgrePlane.h>
#include <OgreResourceGroupManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreBullet.h>
#include <OgreLogManager.h>

LevelScene::LevelScene(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics) : scnMgr(scnMgr), worldPhysics(worldPhysics)
{} // takes screen manager and the game dynamic as an argument

void LevelScene::load()
{
    // create pseudo root node for the level
    rootNode = scnMgr.getRootSceneNode()->createChildSceneNode("LevelRoot");

    // load the DotScene
    rootNode->loadChildren("testScene.scene");

    createColliders(rootNode); // create the colliders and attach them to all objects that need it
    createLighting(); // create the lighting
}

void LevelScene::createLighting()
{
    // create light
    Ogre::Light* light = scnMgr.createLight("LevelMainLight");

    // create the light node
    Ogre::SceneNode* lightNode = rootNode->createChildSceneNode();

    // set position of the light
    lightNode->setPosition(0, 10, 15);

    // attach light to the node
    lightNode->attachObject(light);
}

void LevelScene::createColliders(Ogre::SceneNode* node)
{
    // iterate through all attached objects of the node that was passed in
    for (const auto& entry : node->getAttachedObjects())
    {
        // get the next object
        auto* entity = dynamic_cast<Ogre::Entity*>(entry);

        // if not an entity, then go to the next object
        if (!entity) continue;

        // get the value of the object binding that is "bulletCollider"
        const Ogre::Any& value = entity->getUserObjectBindings().getUserAny("bulletCollider");

        // if there is no value then skip
        if (!value.has_value()) continue;

        // get the string value of the object binding
        const Ogre::String collider = Ogre::any_cast<Ogre::String>(value);

        // get the correct collider type from the string
        Ogre::Bullet::ColliderType type;
        if (collider == "box")
            type = Ogre::Bullet::CT_BOX;
        else if (collider == "trimesh")
            type = Ogre::Bullet::CT_TRIMESH;
        else if (collider == "hull")
            type = Ogre::Bullet::CT_HULL;
        else
        {
            // throw an error if there is a different value in there
            Ogre::LogManager::getSingleton().logWarning(
                "Invalid bulletCollider value '" + collider +
                "' on entity '" + entity->getName() + "'");
            continue;
        }

        // get the value of the mass binding, if any
        const Ogre::Any& massBinding = entity->getUserObjectBindings().getUserAny("mass");

        // make sure the binding isnt empty
        if (!massBinding.has_value()) continue;

        // cast the binding to a float value
        const float mass = Ogre::any_cast<float>(massBinding);
        
        // create the rigidbody attached to the entity with the correct body type and mass
        btRigidBody* newRB = worldPhysics.addRigidBody(mass, entity, type);

        // store a reference to the rigidbody in the object
        entity->getUserObjectBindings().setUserAny("bulletRigidBody", Ogre::Any(newRB));

        // print a success message
        Ogre::LogManager::getSingleton().logMessage(
            "Added " + collider + " collider to " +
            entity->getName());
    }

    // iterate through each subtree of the current node and do the same
    for (const auto& entry : node->getChildren())
    {
        createColliders(
            static_cast<Ogre::SceneNode*>(entry));
    }
}

void LevelScene::unload()
{
    // null check on the root node just in case
    if (!rootNode){
        return;
    }

    // GameObjects own their scene nodes, so destroy them before the level root.
    gameObjects.clear();

    // destroy everything else in the scene
    rootNode->destroyAllChildrenAndObjects();

    // destroy the pseudo root
    scnMgr.destroySceneNode(rootNode);

    // set relevant pointers to null
    rootNode = nullptr;
    terrainNode = nullptr;

    // destroy the plane mesh
    auto& meshManager = Ogre::MeshManager::getSingleton();

    if (meshManager.resourceExists("LevelTerrainMesh"))
        meshManager.remove("LevelTerrainMesh");
}

LevelScene::~LevelScene()
{
    unload();
}

void LevelScene::update(float deltaTime)
{
    // loop through all gameObjects and call update on them
    for (auto& gameObject : gameObjects)
    {
        gameObject->update(deltaTime);
    }
}
