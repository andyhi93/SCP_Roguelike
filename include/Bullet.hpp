#ifndef BULLET_HPP
#define BULLET_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
#include "Collider.hpp"

class Player;
class LevelManager;
class Bullet : public Util::GameObject {
public:
    Bullet(int _damage, GameObject::CollisionLayer _layer, float _speed,int _imageID, glm::vec2 _direction);

    void Update();
    void Move();
    void Hit();
    bool GetIslive();
    bool IsOutOfBounds();
    void getLevelManager(std::shared_ptr<LevelManager> _LevelManager);
    void getPlayer(std::shared_ptr<Player> _Player);
private:
    float damage;
    float speed;
    glm::vec2 direction;
    bool islive = true;
    std::shared_ptr<Collider> m_Collider;
    std::shared_ptr<LevelManager> m_LevelManager;
    std::shared_ptr<Player> m_Player;
};

#endif
