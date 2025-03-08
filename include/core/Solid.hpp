#ifndef SOLID_HPP
#define SOLID_HPP

#include "BoxCollider.hpp"
#include "ColliderManager.hpp"
#include "Object.hpp"

class Solid : public Object {
public:
    std::shared_ptr<BoxCollider> m_collider;

    Solid(glm::vec2 pos, glm::vec2 size)
        : m_collider(std::make_shared<BoxCollider>(pos, size)){
        ColliderManager::GetInstance().RegisterCollider(m_collider);
    }

    virtual ~Solid() {
        ColliderManager::GetInstance().UnregisterCollider(m_collider);
    }

    void SetActive(bool active) {
        m_collider->isActive = active;
    }
};

#endif
