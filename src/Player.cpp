#include "Player.h"

#include <OgreVector.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreBullet.h> 

Player::Player(Ogre::SceneManager& scnMgr, Ogre::Bullet::DynamicsWorld& worldPhysics) : GameObject(scnMgr, scnMgr.getRootSceneNode()->createChildSceneNode("PlayerNode"))
{ // creates the player node and passes it to the game object constructor before doing anything else

    Ogre::Entity* playerMesh = scnMgr.createEntity("baby.mesh"); // create entity mesh

    sceneNode->attachObject(playerMesh); // attach the mesh to the node

    sceneNode->setPosition(0.0f, 2.0f, 0.0f); // move the player above the plane so it does not clip through

    rb = worldPhysics.addRigidBody(10.0f, playerMesh, Ogre::Bullet::CT_CAPSULE); // create and attach the rigid body

    rb->setAngularFactor(btVector3(0, 0, 0)); // prevent the capsule from topping which makes the baby roll
}

void Player::update(float deltaTime)
{
    // enter a state machine dependant on what state the player is currently in
    switch (currentState)
    {
    case NONE: // if the state is none, then the player is free to move about or enter any other states
        if (input.lClickPressed){ // if the player pressed left click then start charging the left spell
            SetCurrentState(CHARGING_LEFT_SPELL);
        }
        else if (input.rClickPressed){ // if player pressed right click then start charging right spell
            SetCurrentState(CHARGING_RIGHT_SPELL);
        }
        else { // otherwise player can move freely
            movePlayer(deltaTime);
        }
        break;
    
    case CHARGING_LEFT_SPELL: // if the player is charging the left spell
        // check if the player is still holding left click
        if (input.lClickReleased){  // if no, then stop charging the spell
            SetCurrentState(NONE);
            leftSpellChargeTimer = 0.0f;
        }
        else { // otherwise continue
            ChargeLeftSpell(deltaTime);
        }
        break;
    
    case CHARGING_RIGHT_SPELL: // if the player is charging the right spell
        // check if the player is still holding right click
        if (input.rClickReleased){  // if no, then stop charging the spell
            SetCurrentState(NONE);
            rightSpellChargeTimer = 0.0f;
        }
        else { // otherwise continue
            ChargeRightSpell(deltaTime);
        }
        break;
        
    default:
        break;
    }
}

void Player::movePlayer(float deltaTime)
{
    Ogre::Vector3 direction = Ogre::Vector3::ZERO; // create movement vector

    // apply movement from input
    if (input.forward) {direction.z -= 1.0f;}
    if (input.back) {direction.z += 1.0f;}
    if (input.left) {direction.x -= 1.0f;}
    if (input.right) {direction.x += 1.0f;}

    // move the player object
    if (direction.isZeroLength() == false){ // only move if the player presses movement input

        // normalise direction
        direction.normalise();

        // update the rigidbody velocity
        btVector3 velocity = rb->getLinearVelocity();
        rb->setLinearVelocity(btVector3(direction.x * moveSpeed, velocity.y(), direction.z * moveSpeed));

        // get the target rotation of the player
        Ogre::Quaternion targetRotation = Ogre::Vector3::UNIT_Z.getRotationTo(direction);

        // convert the target rotation into a bullet transform
        btTransform transform = rb->getCenterOfMassTransform();
        transform.setRotation(Ogre::Bullet::convert(targetRotation));

        // apply the transform to the rigidbody
        rb->setCenterOfMassTransform(transform);
        rb->getMotionState()->setWorldTransform(transform);

        // mark the player as active
        rb->activate(true);
    }
}

void Player::SetInput(InputState newInput)
{
    input = newInput;
}

void Player::SetCurrentState(PlayerState newState)
{
    currentState = newState;
}

PlayerState Player::GetCurrentState()
{
    return currentState;
}

void Player::ChargeLeftSpell(float deltaTime)
{
    // if the left spell timer is less than the charge time then increment it
    if (leftSpellChargeTimer < leftSpellChargeTime){
        leftSpellChargeTimer += deltaTime;
    }
    else { // reset the timer and cast the spell
        leftSpellChargeTimer = 0.0f;
        SetCurrentState(CASTING_LEFT_SPELL);
    }
}

void Player::ChargeRightSpell(float deltaTime)
{
    // if the right spell timer is less than the charge time then increment it
    if (rightSpellChargeTimer < rightSpellChargeTime){
        rightSpellChargeTimer += deltaTime;
    }
    else { // reset the timer and cast the spell
        rightSpellChargeTimer = 0.0f;
        SetCurrentState(CASTING_RIGHT_SPELL);
    }
}
