#include "Core/Actor.hpp"
#include "cmath"
#include <Player.hpp>
Actor::Actor(glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(m_Transform.translation, size)) {
    ColliderManager::GetInstance().RegisterCollider(m_collider);
}
void Actor::SetDead() {
    isDead = true;
    ColliderManager::GetInstance().UnregisterCollider(m_collider);
    this->SetZIndex(this->GetZIndex() - 0.2f);
}

void Actor::MoveX(float amount) {
    auto OtherSolidCol = CheckCollisionWithSolids();
    auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor) : nullptr;
    if(!m_collider->isTrigger && (m_Transform.translation.x > 830 || m_Transform.translation.x < -830)){
        std::cout << "Out of map\n";
        m_Transform.translation.x = m_Transform.translation.x>830? 800:-800;
        m_collider->position.x = m_Transform.translation.x;
    }
    if (OtherSolid && (m_collider->isTrigger && (OtherSolidCol->tag == "Wall" || OtherSolidCol->tag == "Door"))){
        int sign = (-m_Transform.translation.x) < 0 ? -1 : 1;
        while (m_collider->CheckCollisionEdge(OtherSolidCol)) {
            m_Transform.translation.x += sign;
        }
        m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
    }
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor) : nullptr;
    auto PlayerActor = std::dynamic_pointer_cast<Player>(m_collider->parentActor);
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
        float diff = (m_Transform.translation.x - OtherActor->m_Transform.translation.x);
        if (diff > 0) m_Transform.translation.x += 1;
        else if (diff < 0) m_Transform.translation.x -= 1;
        m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
    }
    xRemainder += amount;
    int move = (int)round(xRemainder);
    if (move != 0)
    {
        xRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        m_collider->position.x += sign;
        while (move != 0)
        {
            if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                break;
            }
            else if (!CheckCollisionWithSolids() || (m_collider->isTrigger && !(PlayerActor)))
            {
                m_Transform.translation.x += sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                move -= sign;
            }
            else {
                m_Transform.translation.x -= sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                break;
            }
        }
    }
}

void Actor::MoveY(float amount) {
    auto OtherSolidCol = CheckCollisionWithSolids();
    auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor) : nullptr;
    auto PlayerActor = std::dynamic_pointer_cast<Player>(m_collider->parentActor);
    if (!m_collider->isTrigger && !PlayerActor && (m_Transform.translation.y > 354 || m_Transform.translation.y < -420)) {
        m_Transform.translation.y=m_Transform.translation.y > 354 ? 300 : -373;
        m_collider->position.y = m_Transform.translation.y;
    }
    if (OtherSolid && (!m_collider->isTrigger && (OtherSolidCol->tag == "Wall" ||
        OtherSolidCol->tag == "Door1" || OtherSolidCol->tag == "Door2" || OtherSolidCol->tag == "Door3" || OtherSolidCol->tag == "Door4")))//remember x
    {
        int sign = (-m_Transform.translation.y) < 0 ? -1 : 1;
        while (m_collider->CheckCollisionEdge(OtherSolidCol)) {
            m_Transform.translation.y += sign;
        }
        m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
    }
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
        float diff = (m_Transform.translation.y - OtherActor->m_Transform.translation.y);
        if (diff > 0) m_Transform.translation.y += 1;
        else if (diff < 0) m_Transform.translation.y -= 1;
        m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
    }
    yRemainder += amount;
    int move = (int)round(yRemainder);
    if (move != 0)
    {
        yRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        m_collider->position.y += sign;
        while (move != 0)
        {
            if (!playerIsDashing &&!CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                break;
            }
            else if (!CheckCollisionWithSolids() || (m_collider->isTrigger&& !(PlayerActor)))
            {
                m_Transform.translation.y += sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                move -= sign;
            }
            else {
                m_Transform.translation.y -= sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                break;
            }
        }
    }
}

std::shared_ptr<BoxCollider> Actor::CheckCollisionWithSolids() {
    std::vector<std::shared_ptr<BoxCollider>> solids;
    if(canFly) solids = ColliderManager::GetInstance().GetFlyColliders();
    else solids = ColliderManager::GetInstance().GetSolidColliders();
    //std::cout << "solid size: " << solids.size() << std::endl;
    for (auto& solid : solids) {
        if (m_collider->CheckCollision(solid) && solid->isActive) {
            return solid;
        }
    }
    return nullptr;
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