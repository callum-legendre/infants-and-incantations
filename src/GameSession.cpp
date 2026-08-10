#include "GameSession.h"

GameSession::GameSession(Ogre::SceneManager* scnMgr) : scnMgr(scnMgr);
{
    CreatePlayer();
    CreatePlayerCamera();
    LoadLevel();
}
