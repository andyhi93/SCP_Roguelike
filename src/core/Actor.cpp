#include "Core/Actor.hpp"
Actor::Actor(glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(m_Transform.translation, size)) {
    //m_collider->parentActor = shared_from_this();
    ColliderManager::GetInstance().RegisterCollider(m_collider);
}

void Actor::MoveX(float amount) {
    int sign = (amount > 0) ? 1 : -1;

    while (amount != 0) {
        glm::vec2 newPos = m_Transform.translation + glm::vec2(sign , 0);
        m_collider->position = newPos;

        if (!CheckCollisionWithSolids()) {
            m_Transform.translation.x += sign ;
            m_collider->position = m_Transform.translation;
            amount -= sign;
        }
        else {
            m_collider->position -= newPos;
            break; // 撞到障礙物
        }
    }
}

void Actor::MoveY(float amount) {
    int sign = (amount > 0) ? 1 : -1;

    while (amount != 0) {
        glm::vec2 newPos = m_Transform.translation + glm::vec2(0, sign);
        m_collider->position = newPos;

        if (!CheckCollisionWithSolids()) {
            m_Transform.translation.y += sign;
            m_collider->position = m_Transform.translation;
            amount -= sign;
        }
        else {
            m_collider->position -= newPos;
            break; // 撞到障礙物
        }
    }
}

bool Actor::CheckCollisionWithSolids() {
    auto solids = ColliderManager::GetInstance().GetSolidColliders();
    for (auto& solid : solids) {
        if (m_collider->CheckCollision(solid)) {
            return true;
        }
    }
    return false;
}