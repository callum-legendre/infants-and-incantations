/*
This file contains both the container class that will actually be bound to the the scenenode
The container object will be the function to actually store each component, and will contain member functions for this end.
*/

#include "components/Component.h"

#include <unordered_map>
#include <typeindex>
// #include <memory>
// #include <utility>

namespace Ogre
{
    class SceneNode;
}

class ComponentManager
{
    public:

    ComponentManager(Ogre::SceneNode& parent) : mParent(parent) {}

    // add component method
    template <typename ComponentType, typename... Args> // template function type declaration
    void addComponent(Args&&... args)
    {
        mComponentMap[typeid(ComponentType)] = std::make_unique<ComponentType>(std::forward<Args>(args)...);
    }

    // get component method
    template <typename ComponentType> // also templated
    ComponentType* getComponent()
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

    // an update member function maybe?

    private:

    // component hashmap (should be unordered for O(1) indexing complexity)
    std::unordered_map<std::type_index, std::unique_ptr<Component>> mComponentMap;

    Ogre::SceneNode& mParent;

};
