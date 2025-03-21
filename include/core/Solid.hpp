#ifndef SOLID_HPP
#define SOLID_HPP

#include "BoxCollider.hpp"
#include "ColliderManager.hpp"
#include "Object.hpp"
#include "Camera.hpp"

class Solid : public Object, public std::enable_shared_from_this<Solid> {
public:
    std::shared_ptr<BoxCollider> m_collider;
    static std::vector<std::shared_ptr<Solid>> walls;
    virtual void Start();
    void Update();

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