#ifndef BULLET_HPP
#define BULLET_HPP

#include "pch.hpp"

#include "Util/Image.hpp"
#include "Core/Actor.hpp"

class Player;
class LevelManager;
class Bullet : public Actor ,public Trigger{
public:
    explicit Bullet(glm::vec2 pos,float _damage,  CollisionLayer _layer, float _speed,int _imageID, glm::vec2 _direction);

    void Start();
    void Update() override;
    void FixedUpdate() override;
    void OnTriggerEnter(std::shared_ptr<BoxCollider> other) override;
    void Move();
    inline bool GetIslive() { return islive; }
    void SetIsLive(bool cmd) { islive = cmd; }
    void OutOfBounds();
    bool isGetLevelManager = false;
    bool isGetPlayer = false;
    CollisionLayer getLayer() { return layer; }

    bool isInCamera = false;
private:
    float damage;
    float speed;
    glm::vec2 direction;
    bool islive = true;
};

#endif
