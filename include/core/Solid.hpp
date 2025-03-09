#ifndef SOLID_HPP
#define SOLID_HPP

#include "BoxCollider.hpp"
#include "ColliderManager.hpp"
#include "Object.hpp"

class Solid : public Object {
public:
    std::shared_ptr<BoxCollider> m_collider;
    static std::vector<std::shared_ptr<BoxCollider>> walls;

    Solid(glm::vec2 pos, glm::vec2 size);
    static void InitializeColliders();
    virtual ~Solid() {
        ColliderManager::GetInstance().UnregisterCollider(m_collider);
    }

    void SetActive(bool active) {
        m_collider->isActive = active;
    }
};

#endif