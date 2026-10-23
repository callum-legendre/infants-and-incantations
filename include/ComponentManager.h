#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H
/*
This file contains both the container class that will actually be bound to the the scenenode
The container object will be the function to actually store each component, and will contain member functions for this end.
*/

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <utility>

namespace Ogre
{
    class SceneNode;
}

class Component;

class ComponentManager
{
    public:

    ComponentManager(Ogre::SceneNode* parent);

    // add component method
    template <typename ComponentType, typename... Args> // template function type declaration
    void AddComponent(Args&&... args)
    {
        mComponentMap[typeid(ComponentType)] = std::make_unique<ComponentType>(std::forward<Args>(args)...);
    }

    // get component method
    template <typename ComponentType> // also templated
    ComponentType* GetComponent()
    {
        // get the type index of the component to get
        std::type_index typeIndex = typeid(ComponentType);

        // search map for it
        auto found = mComponentMap.find(typeIndex);

        // if not found return nullptr
        if (found == mComponentMap.end()){
            return nullptr;
        }
        else {
            // else return the component
            return static_cast<ComponentType*>(found->second.get());
        }
    }

    // returns true if the object has the component, false otherwise
    template <typename ComponentType> // has function that returns a bool
    bool HasComponent()
    {
        if (mComponentMap.find(typeid(ComponentType)) == mComponentMap.end()){
            return false;
        }
        else {
            return true;
        }
    }

    // runs each frame update. Updates all components in the correct order
    void Update();

    void UpdateScripts(float deltaTime);

    Ogre::SceneNode* GetParentNode();

    private:

    // component hashmap (should be unordered for O(1) indexing complexity)
    std::unordered_map<std::type_index, std::unique_ptr<Component>> mComponentMap;

    // reference to the scene node that all components are attached to
    Ogre::SceneNode* mParent;

};

#endif
