#include "GameSession.h"

GameSession::GameSession(Ogre::SceneManager* scnMgr) : scnMgr(scnMgr);
{
    CreatePlayer();
    CreatePlayerCamera();
    LoadLevel();
}

void GameSession::CreatePlayer()
{
    Ogre::Entity* playerMesh = scnMgr->createEntity("baby.mesh"); // create entity mesh

    Ogre::SceneNode* playerNode = scnMgr->getRootSceneNode()->createChildSceneNode("PlayerNode"); // create the player node

    playerNode->attachObject(playerMesh); // attach the mesh to the node
    
    player = std::make_unique<Player>(playerNode); // create the player object using the ogre node
}

void GameSession:CreatePlayerCamera()
{
    // create the camera node and move it to correct starting position
    Ogre::SceneNode* cameraNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cameraNode->setPosition(0, 0, 15);
    cameraNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);
 
    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("PlayerCamera");
    cam->setNearClipDistance(5); // specific to this sample
    cam->setAutoAspectRatio(true);
    cameraNode->attachObject(cam);

    // create the camera GameObject
    camera = std::make_unique<PlayerCamera>(cameraNode, player->GetSceneNode());
}

void GameSession::LoadLevel()
{
    // unload the previous level if one is loaded
    if (currentScene != nullptr){
        currentScene->unload();
    }

    // instanciate the level
    currentScene = std::make_unique<LevelScene>(*scnMgr);
    
    // load the level
    currentScene->load();
}

void GameSession update(float deltaTime)
{
    // update the player and the player camera
    player->update(deltaTime);
    playerCamera->update(deltaTime);
    currentScene->update(deltaTime);
}
