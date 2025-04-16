#include "Core/ColliderManager.hpp"
#include <Core/Solid.hpp>
#include "Player.hpp"
#include <iostream>
#include <Util/Input.hpp>

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
    // 移除不活躍的 collider
    for (auto it = currentColliders.begin(); it != currentColliders.end(); ) {
        auto sharedCol = it->lock();  // 嘗試從 weak_ptr 轉換為 shared_ptr
        if (!sharedCol || !sharedCol->isActive) {
            it = currentColliders.erase(it);
        }
        else {
            ++it;
        }
    }

    // 加入新的活躍 collider
    for (const auto& col : colliders) {
        if (col && col->isActive) {
            // 確保將有效的 weak_ptr 添加到集合中
            currentColliders.insert(col);  // 添加 weak_ptr
        }
    }
}

void ColliderManager::DebugCheckDuplicates() {
    std::unordered_map<int, std::vector<std::shared_ptr<BoxCollider>>> idMap;

    for (const auto& col : currentColliders) {
        auto sharedCol = col.lock();  // 確保 col 是有效的 shared_ptr
        if (sharedCol) {
            idMap[sharedCol->id].push_back(sharedCol);
        }
    }

    std::cout << "\n[Debug] === Duplicate Collider Check ===\n";
    for (const auto& [id, colliders] : idMap) {
        std::cout << "ID: " << id << ", Count: " << colliders.size();
        if (!colliders.empty()) {
            std::cout << ", Tag: " << colliders.front()->tag;
        }
        std::cout << "\n";

        if (colliders.size() > 1) {
            std::cout << "⚠️ Duplicate detected for ID " << id << "\n";
            for (const auto& col : colliders) {
                std::cout << "  -> Ptr: " << col.get() << ", Tag: " << col->tag << "\n";
            }
        }
    }

    std::cout << "Total unique IDs: " << idMap.size() << "\n";
    std::cout << "Total colliders in currentColliders: " << currentColliders.size() << "\n";
    int wallCount = 0;
    for (const auto& col : currentColliders) {
        auto sharedCol = col.lock();
        if (sharedCol && sharedCol->tag == "Wall") {
            ++wallCount;
        }
    }
    std::cout << "Total Wall colliders: " << wallCount << "\n";
}

void ColliderManager::UpdateCollisions() {
    //std::cout << currentColliders.size() << "\n";
    for (auto it1 = currentColliders.begin(); it1 != currentColliders.end(); ++it1) {
        auto col1 = it1->lock();
        if (!col1 || !col1->isActive) continue;

        auto it2 = it1;
        ++it2;
        for (; it2 != currentColliders.end(); ++it2) {
            auto col2 = it2->lock();
            if (!col2 || !col2->isActive) continue;

            if (!isReset) {
                isReset = true;
                break;
            }

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
    if (Util::Input::IsKeyPressed(Util::Keycode::P)) {
        DebugCheckDuplicates();
    }
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
    std::cout << "clear ColliderManager size: " << colliders.size() << "\n";
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
