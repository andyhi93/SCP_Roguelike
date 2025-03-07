#ifndef CHARATER_HPP
#define CHARATER_HPP

#include "Util/GameObject.hpp"

class Charater : public Util::GameObject {
public:
protected:
    enum class CollisionLayer {
        Player = 1,
        Enemy = 2,
        Environment = 4,
    };
    CollisionLayer layer;
};

#endif
