#ifndef PLAYERSCRIPT_H
#define PLAYERSCRIPT_H

#include "components/ScriptComponent.h"

class ComponentManager;

class PlayerScript : ScriptComponent
{
    public:
    PlayerScript(ComponentManager& parent);

    void Update() override;

    private:

    static constexpr float mMoveSpeed = 5.0f; // the multiplier for how fast the player moves (by default)
};

#endif
