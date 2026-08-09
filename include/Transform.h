#ifndef TRANSFORM_H
#define TRANSFORM_H

/*
The transform class that owns and maintains the position, rotation, and scale of all GameObjects
Each GameObject owns its transform. 
*/

#include <OgreVector.h>
#include <OgreQuaternion.h>

class Transform
{
    public:
    Transform() = default; // constructor

    // get methods
    const Ogre::Vector3& GetPosition()  const {return position;}
    const Ogre::Quaternion& GetRotation()  const {return rotation;}
    const Ogre::Vector3& GetScale()  const {return scale;}
    const bool IsDirty()  const {return dirty;}

    // set methods
    void SetPosition(Ogre::Vector3 newPos)
    {
        position = newPos;
        dirty = true;
    }

    void SetRotation(Ogre::Quaternion newRotation)
    {
        rotation = newRotation;
        dirty = true;
    }

    void SetScale(Ogre::Vector3 newScale)
    {
        scale = newScale;
        dirty = true;
    }

    void Translate(Ogre::Vector3 offset)
    {
        position += offset;
        dirty = true;
    }

    void Rotate(Ogre::Quaternion rotationOffset)
    {
        rotation = rotation * rotationOffset;
        dirty = true;
    }

    void ClearDirty() {dirty = false;}

    private:
    // properties
    Ogre::Vector3 position {Ogre::Vector3::ZERO};
    Ogre::Quaternion rotation {Ogre::Quaternion::IDENTITY};
    Ogre::Vector3 scale {Ogre::Vector3::UNIT_SCALE};
    bool dirty = false;
};

#endif
