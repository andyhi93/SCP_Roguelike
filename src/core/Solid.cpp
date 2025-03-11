#include "Core/Solid.hpp"
#include "Core/ColliderManager.hpp"
#include "Core/Actor.hpp"


Solid::Solid(glm::vec2 pos, glm::vec2 size)
    : m_collider(std::make_shared<BoxCollider>(pos, size)) {
    m_collider->isSolid = true;
    ColliderManager::GetInstance().RegisterCollider(m_collider);
}
void Solid::Start() {
    m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Solid::Update() {
    /*auto actorCols = ColliderManager::GetInstance().GetActorColliders();
    for (auto actorCol : actorCols) {
        if (actorCol->tag == "Player") std::cout << "Player \n";
        if (m_collider->CheckCollisionEdge(actorCol)) {
            std::shared_ptr<Actor> actor = std::dynamic_pointer_cast<Actor>(actorCol->parentActor);
            auto direction = actor->m_Transform.translation - m_Transform.translation;
            if (m_collider->size.x > m_collider->size.y) {
                while (m_collider->CheckCollisionEdge(actorCol)) {
                    actor->MoveY(direction.y);
                    std::cout << "Touch y edge \n";
                }
            }
            else {
                while (m_collider->CheckCollisionEdge(actorCol)) {
                    actor->MoveX(direction.x);
                    std::cout << "Touch x edge \n";
                }
            }
        }
    }*/
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
