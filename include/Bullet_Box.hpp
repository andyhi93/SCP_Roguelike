#ifndef BULLETBOX_HPP
#define BULLETBOX_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"

class Bullet_Box : public Util::GameObject {
public:
    Bullet_Box();

    void Update();
    void AutoRemove();
    void RemoveAll();
    std::vector<GameObject> bullets;
private:
    float damage;
};

#endif
