#ifndef PLAYERCAMERASCRIPT_H
#define PLAYERCAMERASCRIPT_H

/*
This is the script for the camera. Right now it just handles how the camera follows the player.
*/

#include "components/ScriptComponent.h"

class ComponentManager;

namespace Ogre
{
    class SceneNode;
}

class PlayerCameraScript : protected ScriptComponent
{
    PlayerCameraScript(ComponentManager& parent, Ogre::SceneNode* player);

    void Update(float deltaTime) override;

    private:

    static constexpr float mFollowSharpness = 8.0f;
    Ogre::SceneNode* mPlayerReference = nullptr;
};

#endif
