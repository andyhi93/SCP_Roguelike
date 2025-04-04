#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <iostream>
#include "BoxCollider.hpp"

class ColliderManager {
public:
    static ColliderManager& GetInstance();

    void RegisterCollider(std::shared_ptr<BoxCollider> collider);

    void UnregisterCollider(std::shared_ptr<BoxCollider> collider);

    void UpdateCollisions();
    void Update();
    std::vector<std::shared_ptr<BoxCollider>> GetSolidColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetWallColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetTableColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetEnemyColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetActorColliders();

    void ClearCollider() {
        for (auto& collider : colliders) {
            collider.reset();
        }
        colliders.clear();
        std::cout << "ColliderManager size: " << colliders.size() << "\n";
    }
private:
    std::vector<std::shared_ptr<BoxCollider>> colliders;
    bool isReset = true;

    ColliderManager() { } // 私有建構子，防止外部創建
    ~ColliderManager() {}

    // 禁止拷貝與賦值
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;
};

#endif