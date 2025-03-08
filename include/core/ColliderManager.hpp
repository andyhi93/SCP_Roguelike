#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <iostream>
#include "BoxCollider.hpp"

class ColliderManager {
public:
    static ColliderManager& GetInstance() {
        static ColliderManager instance; // �T�O�u���@�ӹ��
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

    ColliderManager() {} // �p���غc�l�A����~���Ы�
    ~ColliderManager() {}

    // �T������P���
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;
};

#endif