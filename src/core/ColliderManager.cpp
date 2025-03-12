#include "Core/ColliderManager.hpp"
#include <Core/Solid.hpp>

ColliderManager& ColliderManager::GetInstance() {
    static ColliderManager instance; // 確保只有一個實例
    return instance;
}

void ColliderManager::RegisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.push_back(collider);
    std::shared_ptr<Solid> solid = std::dynamic_pointer_cast<Solid>(collider->parentActor);
    if(collider->tag=="Solid") solid->Start();
}
void ColliderManager::UnregisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void ColliderManager::UpdateCollisions() {
    //std::cout << "colliders size: " << colliders.size() << std::endl;
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            if (!colliders[i]->isActive || !colliders[j]->isActive || colliders[i]->isSolid) continue;
            //if (!colliders[i]->CheckCollision(colliders[j])) continue;
            colliders[i]->HandleCollision(colliders[j]);
            if (colliders[j]->isSolid) continue;
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
    auto SolidCols=GetSolidColliders();
    for (auto solidCol : SolidCols) {
        std::shared_ptr<Solid> solid = std::dynamic_pointer_cast<Solid>(solidCol->parentActor);
        if (solidCol->tag=="Solid") solid->Update();
        else if (solidCol->tag == "Solid"&&!solid)std::cout << "nullptr\n";
    }
}