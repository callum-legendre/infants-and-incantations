#include "scripts/PlayerScript.h"
#include "ComponentManager.h"
#include "components/InputComponent.h"
#include "components/PhysicsComponent.h"

#include "OgreBullet.h"
#include "OgreVector.h"

PlayerScript::PlayerScript(ComponentManager& parent) : ScriptComponent(parent) {}

void PlayerScript::Update(float deltaTime)
{
    MovePlayer();
}

void PlayerScript::MovePlayer()
{
    InputComponent* inputComponent = mParent.GetComponent<InputComponent>();
    InputState input = inputComponent->GetInput();

    Ogre::Vector3 direction = Ogre::Vector3::ZERO; // create movement vector

    // apply movement from input
    if (input.forward) {direction.z -= 1.0f;}
    if (input.back) {direction.z += 1.0f;}
    if (input.left) {direction.x -= 1.0f;}
    if (input.right) {direction.x += 1.0f;}

    PhysicsComponent* physicsComponent = mParent.GetComponent<PhysicsComponent>();
    physicsComponent->ApplyMovement(direction, mMoveSpeed);
}
