#include "Core/ColliderManager.hpp"
#include <Core/Solid.hpp>
#include "Player.hpp"
ColliderManager& ColliderManager::GetInstance() {
    static ColliderManager instance;
    return instance;
}

void ColliderManager::RegisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.push_back(collider);
    isReset = false;
}
void ColliderManager::UnregisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void ColliderManager::UpdateCollisions() {
    //std::cout << "colliders size: " << colliders.size() << std::endl;
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            if (!isReset) {
                isReset = true;
                break;
            }
            std::shared_ptr<Player> PlayerActor=nullptr;
            if(colliders[i]->tag=="Player") PlayerActor = std::dynamic_pointer_cast<Player>(colliders[i]->parentActor.lock());
            if (PlayerActor && PlayerActor->GetIsInvincible() && (colliders[j]->tag == "Item" 
                || colliders[j]->tag == "Door1" || colliders[j]->tag == "Door2" || colliders[j]->tag == "Door3" || colliders[j]->tag == "Door0"
                || colliders[j]->tag == "Table") && colliders[i]->isActive && colliders[j]->isActive) {  }
            else if (!colliders[i]->isActive || !colliders[j]->isActive ) continue;
            colliders[i]->HandleCollision(colliders[j]);
            colliders[j]->HandleCollision(colliders[i]);
        }
    }
}
std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetSolidColliders() {
    std::vector<std::shared_ptr<BoxCollider>> solids;
    for (auto& col : colliders) {
        if (col->isActive && !col->isTrigger && col->isSolid) {
            solids.push_back(col);
        }
    }
    //std::cout << "solid size: " << solids.size()<<"\n";
    return solids;
}
std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetWallColliders() {
    std::vector<std::shared_ptr<BoxCollider>> solids;
    for (auto& col : colliders) {
        if (col->isActive && !col->isTrigger && col->isSolid && 
            (col->tag == "Wall" || col->tag == "Door0" || col->tag == "Door1" || col->tag == "Door2" || col->tag == "Door3")) {
            solids.push_back(col);
        }
    }
    //std::cout << "solid size: " << solids.size()<<"\n";
    return solids;
}
std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetActorColliders() {
    std::vector<std::shared_ptr<BoxCollider>> actors;
    for (auto& col : colliders) {
        if (col->isActive && !col->isTrigger && !col->isSolid) {
            actors.push_back(col);
        }
    }
    return actors;
}
std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetEnemyColliders() {
    std::vector<std::shared_ptr<BoxCollider>> enemys;
    for (auto& col : colliders) {
        if (col->isActive &&col->tag=="Enemy" && !col->isSolid) {
            enemys.push_back(col);
        }
    }
    return enemys;
}
std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetTableColliders() {
    std::vector<std::shared_ptr<BoxCollider>> tables;
    for (auto& col : colliders) {
        if (col->isActive &&col->isSolid) {
            tables.push_back(col);
        }
    }
    return tables;
}
void ColliderManager::Update() {
    UpdateCollisions();
    /*auto SolidCols = GetWallColliders();
    int i = 0;
    for (auto solidCol : SolidCols) {
        std::shared_ptr<Solid> solid = std::dynamic_pointer_cast<Solid>(solidCol->parentActor.lock());
        if (solid) solid->Update();
        if (solid && solid->m_collider->tag == "Wall") i += 1;
    }
    std::cout << "Wall count: " << i << "\n";*/
}