#include "Core/Actor.hpp"
#include "cmath"
Actor::Actor(glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(m_Transform.translation, size)) {
    ColliderManager::GetInstance().RegisterCollider(m_collider);
}

void Actor::MoveX(float amount) {
    xRemainder += amount;
    int move = (int)round(xRemainder);
    if (move != 0)
    {
        xRemainder -= move;
        int sign = move > 0 ? 1 : -1; 
        glm::vec2 newPos = m_Transform.translation + glm::vec2(sign, 0);
        m_collider->position = newPos;
        while (move != 0)
        {
            if (!CheckCollisionWithSolids() || m_collider->isTrigger)
            {
                m_Transform.translation.x += sign;
                m_collider->position = m_Transform.translation + m_collider->offset;
                move -= sign;
            }
            else {
                auto otherActor = CheckCollisionWithActors();
                if (otherActor) {
                    OnCollisionEnter(otherActor);
                    break;
                }
                else {
                    m_Transform.translation.x -= sign;
                    m_collider->position -= newPos;
                    break;
                }
            }
        }
    }
}

void Actor::MoveY(float amount) {
    xRemainder += amount;
    int move = (int)round(xRemainder);
    if (move != 0)
    {
        xRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        glm::vec2 newPos = m_Transform.translation + glm::vec2(0, sign);
        m_collider->position = newPos;
        while (move != 0)
        {
            if (!CheckCollisionWithSolids() || m_collider->isTrigger)
            {
                m_Transform.translation.y += sign;
                m_collider->position = m_Transform.translation+m_collider->offset;
                move -= sign;
            }
            else if (CheckCollisionWithActors() && !m_collider->isTrigger) {
                m_Transform.translation.x -= sign;
                m_collider->position -= newPos;
                break;
            }
            else
            {
                m_Transform.translation.y -= sign;
                m_collider->position -= newPos;
                break;
            }
        }
    }
}

bool Actor::CheckCollisionWithSolids() {
    auto solids = ColliderManager::GetInstance().GetSolidColliders();
    //std::cout << "solid size: " << solids.size() << std::endl;
    for (auto& solid : solids) {
        if (m_collider->CheckCollision(solid)) {
            return true;
        }
    }
    return false;
}
std::shared_ptr<BoxCollider> Actor::CheckCollisionWithActors() {
    auto actors = ColliderManager::GetInstance().GetActorColliders();
    for (auto& actor : actors) {
        if (m_collider != actor && m_collider->CheckCollision(actor) && !actor->isTrigger) {
            return actor;
        }
    }
    return nullptr;
}