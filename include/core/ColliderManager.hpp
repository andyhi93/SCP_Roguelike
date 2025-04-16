#ifndef COLLIDER_MANAGER_HPP
#define COLLIDER_MANAGER_HPP

#include <vector>
#include <unordered_set>
#include <memory>
#include "BoxCollider.hpp"

struct BoxColliderHasher {
    std::size_t operator()(const std::weak_ptr<BoxCollider>& collider) const {
        auto sharedCollider = collider.lock();  // �����ഫ�� shared_ptr
        if (sharedCollider) {
            return std::hash<int>{}(sharedCollider->id);  // ���]�C�� BoxCollider ���ߤ@�� id
        }
        return 0;  // �p�G�L�k�ഫ�� shared_ptr�A�h��^ 0
    }
};
// �Ω��������c��
struct BoxColliderEqual {
    bool operator()(const std::weak_ptr<BoxCollider>& lhs, const std::weak_ptr<BoxCollider>& rhs) const {
        auto left = lhs.lock();
        auto right = rhs.lock();

        // �ˬd lock �O�_���\
        if (left && right) {
            return left->id == right->id;  // ���]�� id �ݩ�
        }

        return false;  // �p�G���@��O�ū��СA�h�������۵�
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
    std::unordered_set<std::weak_ptr<BoxCollider>, BoxColliderHasher, BoxColliderEqual> currentColliders;

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
