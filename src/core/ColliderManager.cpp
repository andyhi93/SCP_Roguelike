#include "Core/ColliderManager.hpp"

ColliderManager& ColliderManager::GetInstance() {
    static ColliderManager instance; // 確保只有一個實例
    return instance;
}

void ColliderManager::RegisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.push_back(collider);
}
void ColliderManager::UnregisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void ColliderManager::UpdateCollisions() {
    //std::cout << "colliders size: " << colliders.size() << std::endl;
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            if (colliders[i]->tag == "") std::cout << "Empty tag" << std::endl;
            if (!colliders[i]->isSolid) {
                colliders[i]->HandleCollision(colliders[j]);
            }
            if (!colliders[j]->isSolid){
                colliders[j]->HandleCollision(colliders[i]);
            }
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
void ColliderManager::Update() {
    UpdateCollisions();
}