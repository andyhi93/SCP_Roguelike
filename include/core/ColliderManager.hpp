#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <unordered_set>
#include <memory>
#include "BoxCollider.hpp"

struct BoxColliderHasher {
    std::size_t operator()(const std::weak_ptr<BoxCollider>& collider) const {
        auto sharedCollider = collider.lock();  // 嘗試轉換為 shared_ptr
        if (sharedCollider) {
            return std::hash<int>{}(sharedCollider->id);  // 假設每個 BoxCollider 有唯一的 id
        }
        return 0;  // 如果無法轉換為 shared_ptr，則返回 0
    }
};
// 用於比較的結構體
struct BoxColliderEqual {
    bool operator()(const std::weak_ptr<BoxCollider>& lhs, const std::weak_ptr<BoxCollider>& rhs) const {
        auto left = lhs.lock();
        auto right = rhs.lock();

        // 檢查 lock 是否成功
        if (left && right) {
            return left->id == right->id;  // 假設有 id 屬性
        }

        return false;  // 如果任一方是空指標，則視為不相等
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
    std::unordered_set<std::weak_ptr<BoxCollider>, BoxColliderHasher, BoxColliderEqual> currentColliders;

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
