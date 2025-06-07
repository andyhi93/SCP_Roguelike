#include "Core/Actor.hpp"
#include "cmath"
#include <Player.hpp>
Actor::Actor(glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(m_Transform.translation, size)) {
    ColliderManager::GetInstance().RegisterCollider(m_collider);
    this->AddChild(m_collider->boxImage);
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
            std::cout << "Out of map OneX\n";
            m_Transform.translation.x = m_Transform.translation.x > 830 ? 800 : -800;
            m_collider->position.x = m_Transform.translation.x;
            m_WorldCoord.x = m_Transform.translation.x > 830 ? 800 : -800;
        }
    }
    else if (!m_collider->isTrigger && isCameraOn) {
        auto OtherSolidCol = CheckCollisionWithSolids();
        auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor.lock()) : nullptr;
        if (!PlayerActor && (m_WorldCoord.x > 1145 || m_WorldCoord.x < -1145)) {
            std::cout << "Out of mapX\n";
            m_WorldCoord.x = m_WorldCoord.x > 1145 ? 1140 : -1140;
        }
    }
    xRemainder += amount;
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
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
                glm::vec2 target = FindNearestAvailablePosition(m_collider->position, sign);
                int times = 0;
                while (times < 100 && m_collider->position.x!= target.x && !CheckCollisionWithSolids()) {
                    times++;
                    int newSign = (m_collider->position.x - target.x) > 0 ? 1 : -1;
                    if(!isCameraOn) m_Transform.translation.y -= m_Transform.translation.y > 0 ? 1 : 0;
                    m_WorldCoord.x -= newSign;
                    m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                }
                m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
                break;
            }
            if (!isBoss && !playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                if (xRemainder * OtherActor->xRemainder <= 0 && abs(xRemainder) > abs(OtherActor->xRemainder) && OtherActor->canPushableX) {
                    OtherActor->MoveX(move);
                    OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
                    m_collider->position.x = m_Transform.translation.x + m_collider->offset.x;
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
        if(CheckCollisionWithSolids()) canPushableX = false;
        else canPushableX = true;
    }
}

void Actor::MoveY(float amount) {
    auto PlayerActor = std::dynamic_pointer_cast<Player>(m_collider->parentActor.lock());
    if (!m_collider->isTrigger && !isCameraOn) {
        auto OtherSolidCol = CheckCollisionWithSolids();
        auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor.lock()) : nullptr;
        if (!PlayerActor && (m_Transform.translation.y > 354 || m_Transform.translation.y < -420)) {
            std::cout << "Out of map OneY\n";
            m_Transform.translation.y = m_Transform.translation.y > 354 ? 300 : -373;
            m_collider->position.y = m_Transform.translation.y;
            m_WorldCoord.y = m_Transform.translation.y > 354 ? 300 : -373;
        }
    }
    else if (!m_collider->isTrigger && isCameraOn) {
        auto OtherSolidCol = CheckCollisionWithSolids();
        auto OtherSolid = (OtherSolidCol) ? std::dynamic_pointer_cast<Solid>(OtherSolidCol->parentActor.lock()) : nullptr;
        if (!PlayerActor && (m_WorldCoord.y > 1650 || m_WorldCoord.y < 775)) {
            //std::cout << "Out of mapY\n";
            m_WorldCoord.y = m_WorldCoord.y > 1650 ? 1648 : 776;
        }
    }
    yRemainder += amount;
    auto OtherCollider = CheckCollisionWithActors();
    auto OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
    bool playerIsDashing = (PlayerActor && PlayerActor->isDashing) ? true : false;
    int move = (int)round(yRemainder);
    if (move != 0)
    {
        yRemainder -= move;
        int sign = move > 0 ? 1 : -1;
        while (move != 0)
        {
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
                glm::vec2 target = FindNearestAvailablePosition(m_collider->position,sign);
                int times = 0;
                while (times < 100 && m_collider->position.y != target.y && !CheckCollisionWithSolids()) {
                    times++;
                    int newSign = (m_collider->position.y - target.y) > 0 ? 1 : -1;
                    if (!isCameraOn) m_Transform.translation.y -= m_Transform.translation.y > 0 ? 1 : 0;
                    m_WorldCoord.y -= newSign;
                    m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                }
                m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
                break;
            }
            if (!isBoss && !playerIsDashing && !CheckCollisionWithSolids() && OtherActor && !OtherActor->isDead && !m_collider->isTrigger) {
                if (yRemainder * OtherActor->yRemainder <= 0 && abs(yRemainder) > abs(OtherActor->yRemainder) &&OtherActor->canPushableY) {
                    OtherActor->MoveY(move);
                    OtherActor = (OtherCollider) ? std::dynamic_pointer_cast<Actor>(OtherCollider->parentActor.lock()) : nullptr;
                    m_collider->position.y = m_Transform.translation.y + m_collider->offset.y;
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
        if(CheckCollisionWithSolids()) canPushableY = false;
        else canPushableY = true;
    }
}
glm::vec2 Actor::FindNearestAvailablePosition(glm::vec2 origin, int searchDir) {
    std::shared_ptr<BoxCollider> tempCollider = std::make_shared<BoxCollider>(origin, m_collider->size);
    tempCollider->offset = m_collider->offset;
    bool isTarget = false;

    int radius = 3;
    if (searchDir > 0) {
        for (int i = radius; i > -(radius + 1); i--) {
            for (int j = radius; j > -(radius + 1) ; j--) {
                tempCollider->position = glm::vec2{ m_collider->position.x + i,m_collider->position.y + j };
                if (!CheckCollisionWithSolids(tempCollider)) {
                    origin = tempCollider->position;
                    break;
                }
            }
            if (!CheckCollisionWithSolids(tempCollider)) break;
        }
        if (!isTarget) {
            radius = 5;
            for (int i = radius; i < -(radius + 1); i++) {
                for (int j = radius; j < -(radius + 1); j++) {
                    tempCollider->position = glm::vec2{ m_collider->position.x + i,m_collider->position.y + j };
                    if (!CheckCollisionWithSolids(tempCollider)) {
                        origin = tempCollider->position;
                        break;
                    }
                }
                if (!CheckCollisionWithSolids(tempCollider)) break;
            }
        }
    }
    else {
        for (int i = -radius; i < (radius + 1); i++) {
            for (int j = -radius; j < (radius + 1); j++) {
                tempCollider->position = glm::vec2{ m_collider->position.x + i,m_collider->position.y + j };
                if (!CheckCollisionWithSolids(tempCollider)) {
                    origin = tempCollider->position;
                    break;
                }
            }
            if (!CheckCollisionWithSolids(tempCollider)) break;
        }
        if (!isTarget) {
            radius = 5;
            for (int i = radius; i < -(radius + 1); i++) {
                for (int j = radius; j < -(radius + 1); j++) {
                    tempCollider->position = glm::vec2{ m_collider->position.x + i,m_collider->position.y + j };
                    if (!CheckCollisionWithSolids(tempCollider)) {
                        origin = tempCollider->position;
                        break;
                    }
                }
                if (!CheckCollisionWithSolids(tempCollider)) break;
            }
        }
    }
    return origin;
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
std::shared_ptr<BoxCollider> Actor::CheckCollisionWithSolids(std::shared_ptr<BoxCollider> boxCollider) {
    std::vector<std::shared_ptr<BoxCollider>> solids;
    if (canFly) solids = ColliderManager::GetInstance().GetWallColliders();
    else solids = ColliderManager::GetInstance().GetSolidColliders();
    //std::cout << "solid size: " << solids.size() << std::endl;
    for (auto& solid : solids) {
        if (boxCollider->CheckCollision(solid) && solid->isActive) {
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