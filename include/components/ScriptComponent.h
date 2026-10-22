#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

/*
This is the declaration of a base script class that all scripts will inherit from.
This inheritance pattern is so that they can be managed by the component manager
*/

#include "components/Component.h"

class ComponentManager;

class ScriptComponent : Component
{
    public:
    ScriptComponent(ComponentManager& parent) : Component(parent) {}

    virtual void Update() {}

    private:

};

#endif
