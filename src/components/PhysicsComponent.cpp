#include "components/PhysicsComponent.h"

#include "OgreVector.h"

PhysicsComponent::PhysicsComponent(ComponentManager& parent, btRigidBody& rigidbody) : Component(parent), mRigidBody(rigidbody) 
{}

void PhysicsComponent::ApplyMovement(Ogre::Vector3 direction, float moveSpeed)
{
    // move the player object
    if (direction.isZeroLength() == false){

        // normalise direction
        direction.normalise();

        // update the rigidbody velocity
        btVector3 velocity = mRigidBody.getLinearVelocity();
        mRigidBody.setLinearVelocity(btVector3(direction.x * moveSpeed, velocity.y(), direction.z * moveSpeed));

        // get the target rotation of the player
        Ogre::Quaternion targetRotation = Ogre::Vector3::UNIT_Z.getRotationTo(direction);

        // convert the target rotation into a bullet transform
        btTransform transform = mRigidBody.getCenterOfMassTransform();
        transform.setRotation(Ogre::Bullet::convert(targetRotation));

        // apply the transform to the rigidbody
        mRigidBody.setCenterOfMassTransform(transform);
        mRigidBody.getMotionState()->setWorldTransform(transform);

        // mark the player as active
        mRigidBody.activate(true);
    }
}