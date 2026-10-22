/*
This file contains both the container class that will actually be bound to the the scenenode
The container object will be the function to actually store each component, and will contain member functions for this end.
*/

#include <unordered_map>
#include <typeindex>
// #include <memory>
// #include <utility>

namespace Ogre
{
    class SceneNode;
}

class Component;

class ComponentManager
{
    public:

    ComponentManager(Ogre::SceneNode* parent);

    // creates a specific component stores its
    template <typename ComponentType, typename... Args> void AddComponent(Args&&... args); // template function type declaration

    // returns the component
    template <typename ComponentType> ComponentType* GetComponent(); // also templated

    // returns true if the object has the component, false otherwise
    template <typename ComponentType> bool HasComponent(); // has function that returns a bool

    // runs each frame update. Updates all components in the correct order
    void Update();

    Ogre::SceneNode* GetParentNode();

    private:

    // component hashmap (should be unordered for O(1) indexing complexity)
    std::unordered_map<std::type_index, std::unique_ptr<Component>> mComponentMap;

    // reference to the scene node that all components are attached to
    Ogre::SceneNode* mParent;

};
