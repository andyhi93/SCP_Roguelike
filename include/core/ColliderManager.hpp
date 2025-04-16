#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <unordered_set>
#include <memory>
#include "BoxCollider.hpp"
#include "Object.hpp"

struct BoxColliderHasher {
    std::size_t operator()(const std::shared_ptr<BoxCollider>& collider) const {
        return std::hash<int>{}(collider->id);  // 假設每個 BoxCollider 有唯一的 id
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

    // 清空所有 collider
    void ClearCollider();

private:
    // 使用 unordered_set 存儲弱引用的 BoxCollider
    std::unordered_set<std::shared_ptr<BoxCollider>, BoxColliderHasher, BoxColliderEqual> currentColliders;

    std::vector<std::shared_ptr<BoxCollider>> colliders;
    bool isReset = true;

    // 私有建構子，防止外部創建
    ColliderManager() { }
    ~ColliderManager() {}

    // 禁止拷貝與賦值
    ColliderManager(const ColliderManager&) = delete;
    ColliderManager& operator=(const ColliderManager&) = delete;
};

#endif // COLLIDER_MANAGER_HPP
