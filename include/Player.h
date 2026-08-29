#ifndef PLAYER_H
#define PLAYER_H

/*
The player object that the player controls. 
It exists outside of all of the LevelScenes, so it survives loading and unloading of them. 
*/

#include "GameObject.h"
#include "InputState.h"
#include "Spell.h"

// forward declarations
class btRigidBody;
namespace Ogre
{
    class SceneNode;
    class SceneManager;

    namespace Bullet
    {
        class DynamicsWorld;
    }
}

// define the enum for easy storage and determining of the player's state
enum PlayerState 
{
    CHARGING_PRIMARY_SPELL,
    CASTING_PRIMARY_SPELL,
    CHARGING_SECONDARY_SPELL,
    CASTING_SECONDARY_SPELL,
    NONE,
};

class Player : public GameObject // inherits from GameObject class
{
    public:
    // -- methods --
        Player(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics); // constructor

        // overriden methods
        void update(float deltaTime) override; // update function

        // get methods
        PlayerState GetCurrentState(); // returns the current state of the player

        // set methods
        void SetInput(InputState newInput); // a set input method as the input cannot be passed through the update function (due to how overrides work)
        void SetCurrentState(PlayerState newState); 
        void SetPrimarySpell(std::unique_ptr<Spell>(newSpell)); 
        void SetSecondarySpell(std::unique_ptr<Spell>(newSpell));

    private:
    // -- methods --
        void movePlayer(float deltaTime); // method that moves the player according to the inputs

    // -- properties -- 
        // constants
        static constexpr float moveSpeed = 5.0f; // the multiplier for how fast the player moves (by default)
    
        // unowned properties 
        btRigidBody* rb = nullptr; // the rigidbody of the player (owned by the DynamicsWorld)

        // player input
        InputState input; // stores the inputs of the player so that they are not passed through the update function
        PlayerState currentState = NONE; // the current state of the player, set to none by default

        // spellcasting
        std::unique_ptr<Spell> primarySpell; // current primary spell
        std::unique_ptr<Spell> secondarySpell; // current secondary spell
        float primarySpellTimer = 0.0f; // timer for casting spell one
        float secondarySpellTimer = 0.0f; // timer for casting spell two
};

#endif
