#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <iostream>
#include "BoxCollider.hpp"

class ColliderManager {
public:
    static ColliderManager& GetInstance() {
        static ColliderManager instance; // 確保只有一個實例
        return instance;
    }

    void RegisterCollider(std::shared_ptr<BoxCollider> collider) {
        colliders.push_back(collider);
    }

    void UnregisterCollider(std::shared_ptr<BoxCollider> collider) {
        colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
    }

    void UpdateCollisions() {
        for (size_t i = 0; i < colliders.size(); ++i) {
            for (size_t j = i + 1; j < colliders.size(); ++j) {
                colliders[i]->HandleCollision(colliders[j]);
                colliders[j]->HandleCollision(colliders[i]);
            }
        }
    }
    std::vector<std::shared_ptr<BoxCollider>> GetSolidColliders() {
        std::vector<std::shared_ptr<BoxCollider>> solids;
        for (auto& col : colliders) {
            if (col->isActive && !col->isTrigger && col->isSolid) {
                solids.push_back(col);
            }
        }
        return solids;
    }

private:
    std::vector<std::shared_ptr<BoxCollider>> colliders;

    ColliderManager() {} // 私有建構子，防止外部創建
    ~ColliderManager() {}

    // 禁止拷貝與賦值
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;
};

#endif