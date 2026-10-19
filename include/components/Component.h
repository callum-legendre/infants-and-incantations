#ifndef COMPONENT_H
#define COMPONENT_H

/*
This is the base class implimentation for each component, each component will inherit from it. 
This will essentially just function as an interface for each component type
*/

#include "ComponentManager.h"

class Component
{
    public: 
    Component(ComponentManager& parent) : mParent(parent) {} // constructor
    virtual ~Component() = default; // destructor that is overriden

    virtual void update(float deltaTime) {} // empty update method to be overwritten by the on frame behavior of each component

    private:

    ComponentManager& mParent; // reference to the owner of the component
};

#endif
