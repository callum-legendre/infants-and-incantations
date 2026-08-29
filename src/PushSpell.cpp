#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreVector.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreRay.h>
#include <OgreSphere.h>

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

        // loop through each result and apply the push force to each result 
        for (Ogre::MovableObject* object : result.movables){
            // check if the object is in the cone
                // apply the push to it if so
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
