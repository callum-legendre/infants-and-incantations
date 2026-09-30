#ifndef COMPONENT_H
#define COMPONENT_H

/*
This is the base class implimentation for each component, each component will inherit from it. 
This will essentially just function as an interface for each component type
*/

class Component
{
    public: 
    Component() = default; // constructor
    virtual ~Component() = default; // destructor that is overriden

    virtual void update(float deltaTime) {} // empty update method to be overwritten by the on frame behavior of each component

    private:

};

#endif
