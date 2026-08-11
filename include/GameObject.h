/*
This is the base class for all obejcts that will exist in the game, meaning every object will inherit from this class.
Uses the 'Template Method' for the update function. OnUpdate() is overwritten to provide object specific behavior on each frame update.
All other non-specific frame updates that are needed are done in the update() function to limit repetition of code.
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

namespace Ogre
{
    class SceneManager;
    class SceneNode;
}

class GameObject
{
    public:
    GameObject(Ogre::SceneManager& scnMgr, Ogre::SceneNode* sceneNode); // constructor
    virtual ~GameObject(); // destructor

    // A GameObject exclusively owns its scene node, so it must not be copied or moved.
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = delete;
    GameObject& operator=(GameObject&&) = delete;

    virtual void update(float deltaTime) {} // update that runs every frame, is overriden in child classes
    
    Ogre::SceneNode* GetSceneNode() {return sceneNode;} // get method for the corresponding scene node

    protected: // proporties + methods that can be accessed by child classes
    Ogre::SceneManager& scnMgr;
    Ogre::SceneNode* sceneNode = nullptr;

    private:
};

#endif
