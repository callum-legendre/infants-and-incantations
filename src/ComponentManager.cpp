#include "ComponentManager.h"

#include "components/Component.h"

#include <unordered_map>
#include <typeindex>
// #include <memory>
// #include <utility>


// add component method
template <typename ComponentType, typename... Args> // template function type declaration
void ComponentManager::AddComponent(Args&&... args)
{
    mComponentMap[typeid(ComponentType)] = std::make_unique<ComponentType>(std::forward<Args>(args)...);
}

// get component method
template <typename ComponentType> // also templated
ComponentType* ComponentManager::GetComponent()
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
