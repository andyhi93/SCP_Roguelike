#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <unordered_set>
#include <memory>
#include "BoxCollider.hpp"
#include "Object.hpp"

struct BoxColliderHasher {
    std::size_t operator()(const std::shared_ptr<BoxCollider>& collider) const {
        return std::hash<int>{}(collider->id);  // ���]�C�� BoxCollider ���ߤ@�� id
    }
};
struct BoxColliderEqual {
    bool operator()(const std::shared_ptr<BoxCollider>& lhs, const std::shared_ptr<BoxCollider>& rhs) const {
        return lhs->id == rhs->id;
    }
};


class ColliderManager {
public:
    static ColliderManager& GetInstance();

    void RegisterCollider(std::shared_ptr<BoxCollider> collider);
    void UnregisterCollider(std::shared_ptr<BoxCollider> collider);
    void RefreshCurrentColliders();
    void DebugCheckDuplicates();
    void UpdateCollisions();
    void Update();

    std::vector<std::shared_ptr<BoxCollider>> GetSolidColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetWallColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetTableColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetEnemyColliders();
    std::vector<std::shared_ptr<BoxCollider>> GetActorColliders();

    // �M�ũҦ� collider
    void ClearCollider();

private:
    // �ϥ� unordered_set �s�x�z�ޥΪ� BoxCollider
    std::unordered_set<std::shared_ptr<BoxCollider>, BoxColliderHasher, BoxColliderEqual> currentColliders;

    std::vector<std::shared_ptr<BoxCollider>> colliders;
    bool isReset = true;

    // �p���غc�l�A����~���Ы�
    ColliderManager() { }
    ~ColliderManager() {}

    // �T������P���
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;
};

#endif // COLLIDER_MANAGER_HPP
