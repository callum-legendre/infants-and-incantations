#ifndef PLAYERSCRIPT_H
#define PLAYERSCRIPT_H

#include "components/ScriptComponent.h"

class ComponentManager;

class PlayerScript : public ScriptComponent
{
    public:
    PlayerScript(ComponentManager& parent);

    void Update(float deltaTime) override;

    private:

    void MovePlayer(InputState input);

    void CastSpell(InputState input);

    static constexpr float mMoveSpeed = 5.0f; // the multiplier for how fast the player moves (by default)
};

#endif
