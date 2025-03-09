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
    std::vector<std::shared_ptr<BoxCollider>> GetTableColliders();
private:
    std::vector<std::shared_ptr<BoxCollider>> colliders;

    ColliderManager() { } // �p���غc�l�A����~���Ы�
    ~ColliderManager() {}

    // �T������P���
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;
};

#endif