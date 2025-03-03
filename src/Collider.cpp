#include "Collider.hpp"
#include "LevelManager.hpp"

Collider::Collider(glm::vec2 _pos, glm::vec2 _size){
    position = _pos;//Left Bottom
    size = _size;
}
void Collider::getLevelManager(std::shared_ptr<LevelManager> _LevelManager) {
    m_LevelManager = _LevelManager;
}
bool Collider::CheckCollision(const Collider& a, const Collider& b) {
    return (a.position.x < b.position.x + b.size.x &&
        a.position.x + a.size.x > b.position.x &&
        a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y);
}
std::vector<Collider> Collider::walls = {
    Collider(glm::vec2(840, -470), glm::vec2(10, 335)), // right bottom
    Collider(glm::vec2(840, 53), glm::vec2(10, 340)), // right top
    Collider(glm::vec2(-900, -480), glm::vec2(830, 10)), // bottom left
    Collider(glm::vec2(60, -480), glm::vec2(830, 10)), // bottom right
    Collider(glm::vec2(-900, -470), glm::vec2(60, 335)), // left bottom
    Collider(glm::vec2(-900, 53), glm::vec2(60, 400)), // left top
    Collider(glm::vec2(-900, 414), glm::vec2(770, 10)), // top left
    Collider(glm::vec2(55, 414), glm::vec2(770, 10)), // top right
};
std::vector<Collider> Collider::doors = {
    Collider(glm::vec2(841 ,- 130), glm::vec2(60, 120)), //e
    Collider(glm::vec2(-69 ,- 529), glm::vec2(126, 57)),//s
    Collider(glm::vec2(-902 ,- 131), glm::vec2(60, 123)), //w
    Collider(glm::vec2(-130 ,414), glm::vec2(185, 108)) //n
};
const std::vector<Collider>& Collider::GetWalls() {
    return walls;
}
const std::vector<Collider>& Collider::GetDoors() {
    return doors;
}
glm::vec2 Collider::blockDetect(glm::vec2& velocity) {
    if (CheckCollision(*this, doors[0]) && velocity.x > 0) {
        if (!m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            velocity.x = 0;
        }
    }
    if (CheckCollision(*this, doors[1]) && velocity.y < 0) {
        if (!m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            velocity.y = 0;
        }
    }
    if (CheckCollision(*this, doors[2]) && velocity.x < 0) {
        if (!m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            velocity.x = 0;
        }
    }
    if (CheckCollision(*this, doors[3]) && velocity.y > 0) {
        if (!m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            velocity.y = 0;
        }
    }
    //wall
    if ((CheckCollision(*this, walls[0]) || CheckCollision(*this, walls[1])) && velocity.x > 0) {
        velocity.x = 0;
    }
    if ((CheckCollision(*this, walls[2]) || CheckCollision(*this, walls[3])) && velocity.y < 0) {
        velocity.y = 0;
    }
    if ((CheckCollision(*this, walls[4]) || CheckCollision(*this, walls[5])) && velocity.x < 0) {
        velocity.x = 0;
    }
    if ((CheckCollision(*this, walls[6]) || CheckCollision(*this, walls[7])) && velocity.y > 0) {
        velocity.y = 0;
    }
    return velocity;
}