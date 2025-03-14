#include "Core/ColliderManager.hpp"
#include <Core/Solid.hpp>
#include "Player.hpp"
ColliderManager& ColliderManager::GetInstance() {
    static ColliderManager instance;
    return instance;
}

void ColliderManager::RegisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.push_back(collider);
    std::shared_ptr<Solid> solid = std::dynamic_pointer_cast<Solid>(collider->parentActor);
    if(collider->tag=="Solid") solid->Start();
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
            auto PlayerActor = std::dynamic_pointer_cast<Player>(colliders[i]->parentActor);
            if (PlayerActor && PlayerActor->GetIsInvincible() && (colliders[j]->tag == "Item" 
                || colliders[j]->tag == "Door1" || colliders[j]->tag == "Door2" || colliders[j]->tag == "Door3" || colliders[j]->tag == "Door4"
                || colliders[j]->tag == "Table") && colliders[j]->isActive) {  }
            else if (!colliders[i]->isActive || !colliders[j]->isActive) continue; 

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
        if (solidCol->tag == "Solid") solid->Update();
        else if (solidCol->tag == "Chest" && !solid)  std::cout << "nullptr\n";
    }
}