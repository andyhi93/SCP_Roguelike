#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "BoxCollider.hpp"
#include "ColliderManager.hpp"
#include "Object.hpp"

class Actor : public Object, public std::enable_shared_from_this<Actor> {
public:
    glm::vec2 velocity;
    std::shared_ptr<BoxCollider> m_collider;
    float xRemainder=0, yRemainder=0;

    bool isDead = false;
    Actor(glm::vec2 size);

    ~Actor() {
        ColliderManager::GetInstance().UnregisterCollider(m_collider);
    }

    void MoveX(float amount);

    void MoveY(float amount);

    void SetDead();

    std::shared_ptr<BoxCollider> Actor::CheckCollisionWithSolids();
    std::shared_ptr<BoxCollider> Actor::CheckCollisionWithActors();
};
#endif