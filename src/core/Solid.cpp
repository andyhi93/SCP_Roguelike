#include "Core/Solid.hpp"
#include "Core/ColliderManager.hpp"
#include "Core/Actor.hpp"


Solid::Solid(glm::vec2 pos, glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(pos, size)) {
    m_collider->isSolid = true;
    ColliderManager::GetInstance().RegisterCollider(m_collider);
    relativePos = pos;
}
void Solid::Start() {
    m_collider->parentActor = shared_from_this();
}
void Solid::Update() {
}
/*void Solid::InitializeColliders() {
    for (auto& wall : walls) {
        wall->m_collider->isSolid = true;
        wall->m_collider->tag = "Wall";
        wall->m_collider->parentActor = std::dynamic_pointer_cast<Object>(wall);
    }
}*/
