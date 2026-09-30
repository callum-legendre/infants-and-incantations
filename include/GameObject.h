/*
GameObject is the class that contains the data for all objects that exist in the game. It uses a component system and factories
to allow easy modularity and to prevent diamond inheritance patterns from forming accidentally.
*/

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <utility>

#include "Component.h"

namespace Ogre
{
    class SceneManager;
    class SceneNode;
}

class GameObject
{
    public:
    GameObject() = default; // constructor
    ~GameObject() = default; // destructor (components in unordered map are destroyed automatically because of unique_ptr)

    // add component method
    template <typename ComponentType, typename... Args> // template function type declaration
    void addComponent(Args&&... args)
    {
        // create the component using the passed in args and type, then add it to the hash map
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

    // update method
    void update(float deltaTime)
    {
        // TODO: iterate through all components in specific order???
        for (auto& [type, component] : mComponentMap){
            component->update(deltaTime);
        }
    }

    private:

    // component hashmap (should be unordered for O(1) time complexity)
    std::unordered_map<std::type_index, std::unique_ptr<Component>> mComponentMap;


};

#endif
