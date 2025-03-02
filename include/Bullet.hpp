#ifndef BULLET_HPP
#define BULLET_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Collider.hpp"

class Bullet : public Util::GameObject {
public:
    Bullet(int _damage,int _team, float _speed,int _imageID, glm::vec2 _direction);

    void Update();
    void Move();
    void Hit();
    bool GetIslive();
    bool IsOutOfBounds();
private:
    int team;
    float damage;
    float speed;
    glm::vec2 direction;
    bool islive = true;
    std::shared_ptr<Collider> m_Collider;
};

#endif
