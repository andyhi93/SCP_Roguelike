#include "Core/Solid.hpp"


Solid::Solid(glm::vec2 pos, glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(pos, size)) {
    m_collider->isSolid = true;
    ColliderManager::GetInstance().RegisterCollider(m_collider);
}
std::vector<std::shared_ptr<BoxCollider>> Solid::walls = {
    std::make_shared<BoxCollider>(glm::vec2(845, -303), glm::vec2(10, 335)), // right bottom
    std::make_shared<BoxCollider>(glm::vec2(845, 223), glm::vec2(10, 340)), // right top
    std::make_shared<BoxCollider>(glm::vec2(-485, -475), glm::vec2(830, 10)), // bottom left
    std::make_shared<BoxCollider>(glm::vec2(475, -475), glm::vec2(830, 10)), // bottom right
    std::make_shared<BoxCollider>(glm::vec2(-870, -303), glm::vec2(60, 335)), // left bottom
    std::make_shared<BoxCollider>(glm::vec2(-870, 253), glm::vec2(60, 400)), // left top
    std::make_shared<BoxCollider>(glm::vec2(-485, 427), glm::vec2(830, 10)), // top left
    std::make_shared<BoxCollider>(glm::vec2(440, 427), glm::vec2(770, 10)) // top right
};
void Solid::InitializeColliders() {
    for (auto& wall : walls) {
        wall->isSolid = true;
        wall->tag = "Wall"; 
        ColliderManager::GetInstance().RegisterCollider(wall);
    }
}
