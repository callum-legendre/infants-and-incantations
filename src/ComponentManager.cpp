#include "ComponentManager.h"

#include "components/Component.h"
#include "components/ScriptComponent.h"

ComponentManager::ComponentManager(Ogre::SceneNode* parent) : mParent(parent) {}

Ogre::SceneNode* ComponentManager::GetParentNode()
{
    return mParent;
}

void ComponentManager::UpdateScripts(float deltaTime)
{
    for (auto& [type, component] : mComponentMap) {
        if (auto* script = dynamic_cast<ScriptComponent*>(component.get())) {
            script->Update(deltaTime);
        }
    }
}
