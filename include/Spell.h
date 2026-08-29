#ifndef SPELL_H
#define SPELL_H

/*
This file contains the definition of the base Spell class. All spells that the player can cast are inherited from this class. 
Every spell will have the following charatceristsics:
    - A charge time (the time it takes to charge the spell before casting)
    - A cast effect function
    (more to be added as it beomes relevent)
*/

class Spell
{
public:
    // methods
    Spell(); 
    ~Spell();

    float GetSpellCastTime()  // gets the cast time of the spell
    {
        return spellCastTime;
    };

    virtual void CastSpell() {}; // casts the spell
    
protected:
    // properties
    float spellCastTime;
private:

};

#endif
