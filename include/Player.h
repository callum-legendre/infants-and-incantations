#ifndef PLAYER_H
#define PLAYER_H

/*
The player object that the player controls. 
It exists outside of all of the LevelScenes, so it survives loading and unloading of them. 
*/

#include "GameObject.h"
#include "InputState.h"

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

class Player : public GameObject // inherits from GameObject class
{
    public:
    Player(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics); // constructor

    // overriden methods
    void update(float deltaTime) override; // update function
    void SetInput(InputState newInput); // a set inpit method as the input cannot be passed through the update function (due to how overrides work)

    private:

    // methods
    void movePlayer(float deltaTime);

    // properties
    static constexpr float moveSpeed = 5.0f; // the multiplier for how fast the player moves (by default)
    InputState input; // stores the inputs of the player so that they are not passed through the update function
    btRigidBody* rb = nullptr; // the rigidbody of the player (owned by the DynamicsWorld)
};

#endif
