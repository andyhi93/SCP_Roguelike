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
    // 移除失效或不活躍的 collider
    for (auto it = currentColliders.begin(); it != currentColliders.end(); ) {
        if (!(*it) || !(*it)->isActive) {
            this->RemoveChild(std::static_pointer_cast<Util::GameObject>((*it)->boxImage));
            it = currentColliders.erase(it);
        }
        else {
            ++it;
        }
    }

    // 加入新的活躍 collider
    for (const auto& col : colliders) {
        if (col && col->isActive) {
            this->AddChild(col->boxImage);
            currentColliders.insert(col);  // shared_ptr 直接用
        }
    }
}

void ColliderManager::DebugCheckDuplicates() {
    std::unordered_map<int, std::vector<std::shared_ptr<BoxCollider>>> idMap;

    for (const auto& col : currentColliders) {
        if (col) {
            idMap[col->id].push_back(col);
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
        if (col && col->tag == "Wall") {
            ++wallCount;
        }
    }
    std::cout << "Total Wall colliders: " << wallCount << "\n";
}

void ColliderManager::UpdateCollisions() {
    for (auto it1 = currentColliders.begin(); it1 != currentColliders.end(); ++it1) {
        auto col1 = *it1;
        if (!col1 || !col1->isActive) continue;

        auto it2 = it1;
        ++it2;
        for (; it2 != currentColliders.end(); ++it2) {
            auto col2 = *it2;
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

            if (PlayerActor && (!isDoorOrTable && PlayerActor->GetIsInvincible() )) {
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
    currentColliders.clear();
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
            (col->tag == "Wall" || col->tag == "Door0" || col->tag == "Door1" || col->tag == "Door2" || col->tag == "Door3" || col->tag=="Chest")) {
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
