#ifndef PUSHSPELL_H
#define PUSHSPELL_H

#include "Spell.h"

class Player;
struct InputState;

class PushSpell : public Spell
{
    public:
    PushSpell(Ogre::SceneManager& scnMgr) : Spell(scnMgr, 1.0f) {} // constructor

    // override for the cast method
    void CastSpell(Player& player, const InputState& input) override;

    private:
    // -- properties --
        // spell range properties, these are not constant as they may be modified by other effects
        float spellRange = 8.0f;
        float coneAngleWidth = 45.0f;
        float pushStrength = 15.0f;
};


#endif
