#ifndef BULLET_HPP
#define BULLET_HPP

#include "pch.hpp"

#include "Util/Image.hpp"
#include "Collider.hpp"
#include "Charater.hpp"

class Player;
class LevelManager;
class Bullet : public Charater {
public:
    Bullet(int _damage,  CollisionLayer _layer, float _speed,int _imageID, glm::vec2 _direction);

    void Update();
    void Move();
    void Hit();
    bool GetIslive();
    bool IsOutOfBounds();
    void setLevelManager(std::shared_ptr<LevelManager> _LevelManager);
    bool isGetLevelManager = false;
    void setPlayer(std::shared_ptr<Player> _Player);
    bool isGetPlayer = false;
    CollisionLayer getLayer() { return layer; }
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
