#include "Core/Solid.hpp"
#include "Core/ColliderManager.hpp"
#include "Core/Actor.hpp"


Solid::Solid(glm::vec2 pos, glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(pos, size)) {
    m_collider->isSolid = true;
    ColliderManager::GetInstance().RegisterCollider(m_collider);
    m_Transform.translation = pos;
    m_WorldCoord = pos;
}
void Solid::Start() {
    m_collider->parentActor = shared_from_this();
}
void Solid::Update() {
    //std::cout << "pos: " << m_Transform.translation.x << " ," << m_Transform.translation.y << "\n";
    m_collider->position = m_Transform.translation+m_collider->offset;
}
