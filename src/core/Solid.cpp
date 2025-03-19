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
    m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Solid::Update() {
    if (Camera::GetInstance().isActive) {
        //std::cout << "transform: " << relativePos.x << ", " << relativePos.y << "\n";
        m_Transform.translation = relativePos - Camera::GetInstance().GetCameraWorldCoord();
        this->m_collider->position = m_Transform.translation;
    }
}
std::vector<std::shared_ptr<Solid>> Solid::walls = {
    std::make_shared<Solid>(glm::vec2(845, -303), glm::vec2(10, 335)), // right bottom
    std::make_shared<Solid>(glm::vec2(845, 263), glm::vec2(10, 538)), // right top
    std::make_shared<Solid>(glm::vec2(-485, -475), glm::vec2(830, 10)), // bottom left
    std::make_shared<Solid>(glm::vec2(475, -475), glm::vec2(830, 10)), // bottom right
    std::make_shared<Solid>(glm::vec2(-870, -303), glm::vec2(60, 335)), // left bottom
    std::make_shared<Solid>(glm::vec2(-870, 263), glm::vec2(63, 538)), // left top
    std::make_shared<Solid>(glm::vec2(-485, 425), glm::vec2(830, 100)), // top left
    std::make_shared<Solid>(glm::vec2(440, 425), glm::vec2(770, 100)) // top right
};
void Solid::InitializeColliders() {
    for (auto& wall : walls) {
        wall->m_collider->isSolid = true;
        wall->m_collider->tag = "Wall";
        wall->m_collider->parentActor = std::dynamic_pointer_cast<Object>(wall);
    }
}
