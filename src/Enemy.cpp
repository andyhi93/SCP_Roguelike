#include "Enemy.hpp"
#include "Player.hpp"

Enemy::Enemy(glm::vec2 size) :Actor(size) {
    m_collider->tag = "Enemy";
    layer = CollisionLayer::Enemy;
}
void Enemy::Start() {
    m_collider->parentActor = shared_from_this();
}
void Enemy::SetPlayer(std::shared_ptr<Player> _player) { 
    m_Player = _player; 
}

glm::vec2 Enemy::normalize(glm::vec2 direction) {
    return { direction.x / sqrt(direction.x * direction.x + direction.y * direction.y),direction.y / sqrt(direction.x * direction.x + direction.y * direction.y) };
}
void Enemy::FlipControl() {
    if (m_Player->m_Transform.translation.x-m_Transform.translation.x > 0 && !isFaceRight) {
        m_Transform.scale.x = std::abs(m_Transform.scale.x);
        isFaceRight = true;
    }
    else if(m_Player->m_Transform.translation.x - m_Transform.translation.x <= 0 && isFaceRight){
        m_Transform.scale.x = -std::abs(m_Transform.scale.x);
        isFaceRight = false;
    }
}