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

glm::vec2 Enemy::normalize(glm::vec2 values) {
    float sum =values[0]* values[0]+ values[1]* values[1];
    if (sum == 0) return values; // ����H 0

    values = { values[0] / sum,values[1] / sum };

    return values;
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