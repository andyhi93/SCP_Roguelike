#include "Core/ColliderManager.hpp"
#include <Core/Solid.hpp>
#include "Player.hpp"
#include <iostream>

ColliderManager& ColliderManager::GetInstance() {
    static ColliderManager instance;
    return instance;
}

void ColliderManager::RegisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.push_back(collider);
}

void ColliderManager::UnregisterCollider(std::shared_ptr<BoxCollider> collider) {
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void ColliderManager::RefreshCurrentColliders() {
    currentColliders.clear();  // 清空現有的 colliders

    for (const auto& col : colliders) {
        if (col && col->isActive) {
            currentColliders.insert(col);  // 只加入不重複的 collider
        }
    }
}

void ColliderManager::UpdateCollisions() {
    for (auto it1 = currentColliders.begin(); it1 != currentColliders.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        for (; it2 != currentColliders.end(); ++it2) {
            if (!isReset) {
                isReset = true;
                break;
            }

            auto& col1 = *it1;
            auto& col2 = *it2;

            if (!col1->isActive || !col2->isActive) continue;

            std::shared_ptr<Player> PlayerActor = nullptr;
            if (col1->tag == "Player") {
                PlayerActor = std::dynamic_pointer_cast<Player>(col1->parentActor.lock());
            }

            bool isDoorOrTable = (col2->tag == "Item" || col2->tag == "Door0" || col2->tag == "Door1" ||
                col2->tag == "Door2" || col2->tag == "Door3" || col2->tag == "Table");

            if (PlayerActor && PlayerActor->GetIsInvincible() && isDoorOrTable) {
                continue; // 玩家無敵狀態，不觸發碰撞
            }

            // 正常碰撞處理
            col1->HandleCollision(col2);
            col2->HandleCollision(col1);
        }
    }
}

void ColliderManager::Update() {
    RefreshCurrentColliders();
    UpdateCollisions();
    auto SolidCols = GetWallColliders();
    for (auto solidCol : SolidCols) {
        std::shared_ptr<Solid> solid = std::dynamic_pointer_cast<Solid>(solidCol->parentActor.lock());
        if (solid) {
            solid->Update();
        }
    }
}
void ColliderManager::ClearCollider() {
    for (auto& collider : colliders) {
        collider.reset();
    }
    colliders.clear();
    std::cout << "ColliderManager size: " << colliders.size() << "\n";
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

std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetWallColliders() {
    std::vector<std::shared_ptr<BoxCollider>> solids;
    for (auto& col : colliders) {
        if (col->isActive && !col->isTrigger && col->isSolid &&
            (col->tag == "Wall" || col->tag == "Door0" || col->tag == "Door1" || col->tag == "Door2" || col->tag == "Door3")) {
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

std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetEnemyColliders() {
    std::vector<std::shared_ptr<BoxCollider>> enemys;
    for (auto& col : colliders) {
        if (col->isActive && col->tag == "Enemy" && !col->isSolid) {
            enemys.push_back(col);
        }
    }
    return enemys;
}

std::vector<std::shared_ptr<BoxCollider>> ColliderManager::GetTableColliders() {
    std::vector<std::shared_ptr<BoxCollider>> tables;
    for (auto& col : colliders) {
        if (col->isActive && col->isSolid) {
            tables.push_back(col);
        }
    }
    return tables;
}