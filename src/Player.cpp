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
        if (input.primarySpellPressed){ // if the player pressed the bound buttonk then start charging the primary spell
            SetCurrentState(CHARGING_PRIMARY_SPELL);
        }
        else if (input.secondarySpellPressed){ // if player pressed the bound button then start charging secondary spell
            SetCurrentState(CHARGING_SECONDARY_SPELL);
        }
        else { // otherwise player can move freely
            movePlayer(deltaTime);
        }
        break;
    
    case CHARGING_PRIMARY_SPELL: // if the player is charging the prinmary spell
        // check if the player is still holding the bound button
        if (input.primarySpellReleased && primarySpellTimer >= primarySpell.GetSpellCastTime()){  // if no, then check if the spell is charged
            // if spell is charged then cast it and reset timer
            SetCurrentState(CASTING_PRIMARY_SPELL);
            primarySpellTimer = 0.0f;
        }
        else if (input.primarySpellReleased && primarySpellTimer < primarySpell.GetSpellCastTime()){ // if spell is not charged
            // free player movement and reset timer
            SetCurrentState(NONE);
            primarySpellTimer = 0.0f;
        }
        else { // otherwise continue charging the spell
            primarySpellTimer += deltaTime;
        }
        break;
    
    case CHARGING_SECONDARY_SPELL: // if the player is charging the right spell
        // check if the player is still holding right click
        if (input.secondarySpellPressed && secondarySpellTimer >= secondarySpell.GetSpellCastTime()){  // if no, then check if the spell is charged
            // if spell is charged then cast it and reset timer
            SetCurrentState(CASTING_SECONDARY_SPELL);
            secondarySpellTimer = 0.0f;
        }
        else if (input.secondarySpellPressed && secondarySpellTimer < secondarySpell.GetSpellCastTime()){ // if spell is not charged
            // free player movement and reset timer
            SetCurrentState(NONE);
            secondarySpellTimer = 0.0f;
        }
        else { // otherwise continue charging the spell
            secondarySpellTimer += deltaTime;
        }
        break;

    case CASTING_PRIMARY_SPELL: // once the timer has completed, cast the spell and free the player movement
        primarySpell.CastSpell();
        SetCurrentState(NONE); 
        break;
    
    case CASTING_SECONDARY_SPELL:
        secondarySpell.CastSpell();
        SetCurrentState(NONE);
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

void Player::SetSpellOne(Spell newSpell)
{
    spellOne = newSpell;
}

void Player::SetSpellTwo(Spell newSpell)
{
    spellTwo = newSpell;
}
