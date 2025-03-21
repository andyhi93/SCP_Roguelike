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
    auto PlayerActor = std::dynamic_pointer_cast<Player>(m_collider->parentActor.lock());
    if (!m_collider->isTrigger && !isCameraOn) {
        auto OtherSolidCol = CheckCollisionWithSolids();
        auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor.lock()) : nullptr;
        if (m_Transform.translation.x > 830 || m_Transform.translation.x < -830) {
            std::cout << "Out of map\n";
            m_Transform.translation.x = m_Transform.translation.x > 830 ? 800 : -800;
            m_collider->position.x = m_Transform.translation.x;
            m_WorldCoord.x = m_Transform.translation.x > 830 ? 800 : -800;
        }
        /*if (OtherSolid && (OtherSolidCol->tag == "Wall" ||
            OtherSolidCol->tag == "Door0" || OtherSolidCol->tag == "Door1" || OtherSolidCol->tag == "Door2" || OtherSolidCol->tag == "Door3")) {
            int sign = (m_Transform.translation.x-OtherSolid->m_Transform.translation.x) < 0 ? -1 : 1;
            while (m_collider->CheckCollisionEdge(OtherSolidCol)) {
                m_Transform.translation.x += sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                m_WorldCoord.x += sign;
                std::cout << "stuck wallX\n";
            }
        }*/
    }
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
        float diff = (m_Transform.translation.x - OtherActor->m_Transform.translation.x);
        if (diff > 0) {
            m_Transform.translation.x += 1;
            m_WorldCoord.x += 1;
        }
        else if (diff < 0) {
            m_Transform.translation.x -= 1;
            m_WorldCoord.x -= 1;
        }
        m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
    }
    xRemainder += amount;
    int move = (int)round(xRemainder);
    if (move != 0)
    {
        xRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        while (move != 0)
        {
            m_collider->position.x += sign;
            if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                break;
            }
            else if (!CheckCollisionWithSolids() || (m_collider->isTrigger && !(PlayerActor)))
            {
                m_Transform.translation.x += sign;
                m_WorldCoord.x += sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                move -= sign;
            }
            else {
                m_Transform.translation.x -= sign;
                m_WorldCoord.x -= sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                break;
            }
        }
    }
}

void Actor::MoveY(float amount) {
    auto PlayerActor = std::dynamic_pointer_cast<Player>(m_collider->parentActor.lock());
    if (!m_collider->isTrigger && !isCameraOn) {
        auto OtherSolidCol = CheckCollisionWithSolids();
        auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor.lock()) : nullptr;
        if (!PlayerActor && (m_Transform.translation.y > 354 || m_Transform.translation.y < -420)) {
            m_Transform.translation.y = m_Transform.translation.y > 354 ? 300 : -373;
            m_collider->position.y = m_Transform.translation.y;
            m_WorldCoord.y = m_Transform.translation.y > 354 ? 300 : -373;
        }
        /*if (OtherSolid && ((OtherSolidCol->tag == "Wall" ||
            OtherSolidCol->tag == "Door1" || OtherSolidCol->tag == "Door2" || OtherSolidCol->tag == "Door3" || OtherSolidCol->tag == "Door4"))){
            int sign = (m_Transform.translation.y - OtherSolid->m_Transform.translation.y) < 0 ? -1 : 1;
            while (m_collider->CheckCollisionEdge(OtherSolidCol)) {
                m_Transform.translation.y += sign;
                m_WorldCoord.y += sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                std::cout << "stuck wallY\n";
            }
        }*/
    }
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
        float diff = (m_Transform.translation.y - OtherActor->m_Transform.translation.y);
        if (diff > 0) {
            m_Transform.translation.y += 1;
            m_WorldCoord.y += 1;
        }
        else if (diff < 0) {
            m_Transform.translation.y -= 1;
            m_WorldCoord.y -= 1;
        }
        m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
    }
    yRemainder += amount;
    int move = (int)round(yRemainder);
    if (move != 0)
    {
        yRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        while (move != 0)
        {
            m_collider->position.y += sign;
            if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                break;
            }
            else if (!CheckCollisionWithSolids() || (m_collider->isTrigger && !(PlayerActor)))
            {
                m_Transform.translation.y += sign;
                m_WorldCoord.y += sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                move -= sign;
            }
            else {
                m_Transform.translation.y -= sign;
                m_WorldCoord.y -= sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                break;
            }
        }
    }
}

std::shared_ptr<BoxCollider> Actor::CheckCollisionWithSolids() {
    std::vector<std::shared_ptr<BoxCollider>> solids;
    if(canFly) solids = ColliderManager::GetInstance().GetWallColliders();
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