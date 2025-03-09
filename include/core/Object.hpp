#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Util/GameObject.hpp"

class Object : public Util::GameObject {
public:
    virtual void Update(){}
    virtual void FixedUpdate(){}
    enum class CollisionLayer {
        Player = 1,
        Enemy = 2,
        Environment = 3,
    };
    CollisionLayer layer;
protected:
};

#endif
