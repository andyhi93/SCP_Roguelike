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
    xRemainder += amount;
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    /*if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
        if (xRemainder * OtherActor->xRemainder < 0 && abs(xRemainder) < abs(OtherActor->xRemainder)) {
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
            return;
        }
    }*/
    int move = (int)round(xRemainder);
    if (move != 0)
    {
        xRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        while (move != 0)
        {
            m_collider->position.x += sign;

            if (!CheckCollisionWithSolids() || (m_collider->isTrigger && !(PlayerActor)))
            {
                canPushableX = true;
                m_Transform.translation.x += sign;
                m_WorldCoord.x += sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                move -= sign;
            }
            else {
                canPushableX = false;
                m_Transform.translation.x -= sign;
                m_WorldCoord.x -= sign;
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                break;
            }
            if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                if (xRemainder * OtherActor->xRemainder <= 0 && abs(xRemainder) > abs(OtherActor->xRemainder) && OtherActor->canPushableX) {
                    OtherActor->MoveX(move);
                    OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
                }
                else {
                    sign = m_Transform.translation.x - OtherActor->m_Transform.translation.x > 0 ? 1 : -1;
                    m_Transform.translation.x += sign;
                    m_WorldCoord.x += sign;
                    m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                    return; 
                }
            }
        }
    }
    else {
        canPushableX = true;
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
    yRemainder += amount;
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    /*if (!m_collider->isTrigger && !playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead) {
        if (yRemainder* OtherActor->yRemainder<0 && abs(yRemainder) < abs(OtherActor->yRemainder)) {
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
            return;
        }
    }*/
    int move = (int)round(yRemainder);
    if (move != 0)
    {
        yRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        while (move != 0)
        {
            //if ((xRemainder != 0 || yRemainder != 0)&& PlayerActor) std::cout << "Player xReminder: " << xRemainder << " Player yReminder: " << yRemainder << "\n";
            m_collider->position.y += sign;

            if (!CheckCollisionWithSolids() || (m_collider->isTrigger && !(PlayerActor)))
            {
                canPushableY = true;
                m_Transform.translation.y += sign;
                m_WorldCoord.y += sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                move -= sign;
            }
            else {
                canPushableY = false;
                m_Transform.translation.y -= sign;
                m_WorldCoord.y -= sign;
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                break;
            }
            if (!playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                if (yRemainder * OtherActor->yRemainder <= 0 && abs(yRemainder) > abs(OtherActor->yRemainder) &&OtherActor->canPushableY) {
                    OtherActor->MoveY(move);
                    OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
                }
                else {
                    sign = m_Transform.translation.y - OtherActor->m_Transform.translation.y > 0 ? 1 : -1;
                    m_Transform.translation.y += sign;
                    m_WorldCoord.y += sign;
                    m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                    return; 
                }
            }
        }
    }
    else {
        canPushableY = true;
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