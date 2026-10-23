#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

/*
The component that contains a reference to the object's bullet collider. 
It also contains member functions of code to do with movement for ease of use
*/

#include "components/Component.h"
#include "ComponentManager.h"

#include <OgreBullet.h>

class PhysicsComponent : public Component
{
    public:
    PhysicsComponent(ComponentManager& parent, btRigidBody& rigidbody);

    void ApplyMovement(Ogre::Vector3 direction, float moveSpeed);

    private:

    btRigidBody& mRigidBody;

};

#endif
