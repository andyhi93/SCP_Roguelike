#ifndef BULLET_HPP
#define BULLET_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Bullet.hpp"

class Bullet : public Util::GameObject {
public:
    Bullet(int _damage,int team,float _hp, float _speed,int _imageID);

    void Update();
    void Move();
    void Hit();
    bool get_islive() { return islive; }
private:
    float damage;
    bool islive = true;
};

#endif
