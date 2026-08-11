#include "LevelScene.h" // header file

#include <Ogre.h>
#include <OgreBullet.h>

LevelScene::LevelScene(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics) : scnMgr(scnMgr), worldPhysics(worldPhysics)
{} // takes screen manager and the game dynamic as an argument

void LevelScene::load()
{
    // create pseudo root node for the level
    rootNode = scnMgr.getRootSceneNode()->createChildSceneNode("LevelRoot");

    createTerrain(); // create the level environment
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

void LevelScene::createTerrain() 
{
    // create the plane object
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);

    // create the plane mesh
    Ogre::MeshManager::getSingleton().createPlane(
        "LevelTerrainMesh",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane,
        100.0f,
        100.0f,
        20,
        20,
        true,
        1,
        10.0f,
        10.0f,
        Ogre::Vector3::UNIT_Z);

    // create the entity for the terrain mesh
    Ogre::Entity* terrainEntity = scnMgr.createEntity("LevelTerrainEntity", "LevelTerrainMesh");

    // set the material for the entity
    terrainEntity->setMaterialName("Terrain");

    // create new node and attach the plane onto it
    terrainNode = rootNode->createChildSceneNode("LevelTerrainNode");
    terrainNode->attachObject(terrainEntity);

    // create and attach the rigidbody for the terrain
    worldPhysics.addRigidBody(0.0f, terrainEntity, Ogre::Bullet::CT_TRIMESH); // TEMPORARY FOR TEST SCENE
}

void LevelScene::unload()
{
    // null check on the root node just in case
    if (!rootNode){
        return;
    }

    // destroy everything in the scene
    rootNode->destroyAllChildrenAndObjects();

    // destroy the pseudo root
    scnMgr.destroySceneNode(rootNode);

    // remove all gameobjects stored in the vector safely
    gameObjects.clear();

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