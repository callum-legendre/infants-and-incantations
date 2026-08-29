#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreVector.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRay.h>
#include <OgreSphere.h>
#include <OgreBullet.h>

#include "Spell.h"

class PushSpell : public Spell
{
    public:
    PushSpell(Ogre::SceneManager& scnMgr) : Spell(scnMgr, 1.0f) {} // constructor

    // override for the cast method
    void CastSpell(Player& player, InputState input) override
    { // TODO: when writing the next spell, go through this code and identify repeated sections and move them into a function in the parent class
        // get the player camera
        Ogre::Camera* camera = scnMgr.getCamera("PlayerCamera");

        // convert mouse location into coordinates relative to the window
        float viewportX = static_cast<float>(input.mLocationX) / static_cast<float>(camera->getViewport()->getActualWidth());
        float viewportY = static_cast<float>(input.mLocationY) / static_cast<float>(camera->getViewport()->getActualHeight());
        
        // draw a line from the camera to the cursor position
        Ogre::Ray ray = camera->getCameraToViewportRay(viewportX, viewportY);

        // get the player position
        Ogre::Vector3 playerPos = player.GetSceneNode()->_getDerivedPosition();

        // create a plane that passes through the player
        Ogre::Plane aimPlane(Ogre::Vector3::UNIT_Y, playerPos);

        // get the point where the ray intersects the plane that we jusr created
        auto [didHitPlane, distanceAlongRay] = ray.intersects(aimPlane);

        // make sure that the ray and the plane intersect at some point
        if (!didHitPlane) {return;}

        // get the point that the player is aiming towards
        Ogre::Vector3 aimPoint = ray.getPoint(distanceAlongRay);

        // calculate the direction from the player that the player is aiming at
        Ogre::Vector3 aimDirection = aimPoint - playerPos;
        aimDirection.y = 0.0f; // make the direction horizontal, just in case
        aimDirection.normalise(); // normalise the aim direction so that the dot products can be used

        // create the sphere to query in
        Ogre::Sphere searchArea(playerPos, spellRange);

        // create a query for that sphere
        Ogre::SphereSceneQuery* query = scnMgr.createSphereQuery(searchArea);

        // execute the query and get the results
        Ogre::SceneQueryResult& result = query->execute();

        // calculate the cone boundaries
        float halfConeAngle = coneAngleWidth * 0.5f; // calculate half of the width

        // calculate the dot product limit of the obejct's distances
        float minimumDot = Ogre::Math::Cos(Ogre::Degree(halfConeAngle));

        // loop through each result and apply the push force to each result 
        for (Ogre::MovableObject* object : result.movables){

            // get the scene node of each movable object
            Ogre::SceneNode* objectNode = object->getParentSceneNode();

            // check to make sure the object has a parent
            if (!objectNode) {continue;} // if no parent, then skip them

            // skip the player themself (they will always be returned by the query as they are in the radius)
            if (objectNode == player.GetSceneNode()) {continue;}

            // calculate the vector from the player to the object
            Ogre::Vector3 vectorToObject = objectNode->_getDerivedPosition() - playerPos;
            vectorToObject.y = 0.0f; // dont take into account height above the player TODO, CHANGE THIS LATER AFTER TESTING

            // find the distance from the player to the object
            float objectDistance = vectorToObject.length();

            // check if the object is beyond the spell's range TODO: maybe take this away? How forgiving do I want this spell to be?
            if (objectDistance > spellRange) {continue;}

            // create a normalised vector from the object
            Ogre::Vector3 objectDirection = vectorToObject / objectDistance;

            // run the dotproduct calculation to find the angle from the player 
            float alignment = aimDirection.dotProduct(objectDirection);

            // if the alignment is less than the minimum dot product required, then go to the next object and don't apply force to it
            if (alignment < minimumDot) {continue;}

            // get the pointer to the rigidbody from the object
            const Ogre::Any& bodyReference = object->getUserObjectBindings().getUserAny("bulletRigidBody");

            // null check to ensure that the object actually has a rigidbody
            if (!bodyReference.has_value()) {continue;}

            // cast the pointer to a rigidbody pointer so it can be used like one
            btRigidBody* rb = Ogre::any_cast<btRigidBody*>(bodyReference);

            // create the impule vector
            Ogre::Vector3 impulse = objectDirection * pushStrength;

            // convert the ogre vector into a bullet vector, so it can be applied to the rigidbody
            btVector3 bulletInpulse = Ogre::Bullet::convert(impulse);

            // apply the impulse to the object
            rb->activate(true); // activete the rigidbody
            rb->applyCentralImpulse(bulletInpulse); // apply the impulse to it
        }

        // now that it has executed and results have been processed, the query is no longer needed so destroy it
        scnMgr.destroyQuery(query);

    }

    private:
    // -- properties --
        // spell range properties, these are not constant as they may be modified by other effects
        float spellRange = 8.0f;
        float coneAngleWidth = 45.0f;
        float pushStrength = 15.0f;
};
