#include "Enemy.hpp"
#include "Player.hpp"
#include "random"

Enemy::Enemy(glm::vec2 size) :Actor(size) {
    m_collider->tag = "Enemy";
    layer = CollisionLayer::Enemy;
}
void Enemy::Start() {
    m_collider->parentActor = shared_from_this();
    if (isDropCoin) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, 4);
        int cmd = dis(gen);
        if (cmd == 0 || cmd == 1 || cmd==2) isDropCoin = false;
    }
}

void Enemy::SetPlayer(std::shared_ptr<Player> _player) { 
    m_Player = _player; 
}
std::shared_ptr<BloodCoin> Enemy::GetCoin() {
    auto coin = std::make_shared<BloodCoin>(m_Transform.translation);
    coin->m_Transform.translation = m_Transform.translation - glm::vec2{0, m_collider->size.y/4};
    coin->SetZIndex(GetZIndex() + 0.01f);
    return coin;
}

glm::vec2 Enemy::normalize(glm::vec2 direction) {
    return { direction.x / sqrt(direction.x * direction.x + direction.y * direction.y),direction.y / sqrt(direction.x * direction.x + direction.y * direction.y) };
}
void Enemy::FlipControl() {
    if (m_Player->m_Transform.translation.x-m_Transform.translation.x > 0.01f && !isFaceRight) {
        m_Transform.scale.x = std::abs(m_Transform.scale.x);
        isFaceRight = true;
    }
    else if(m_Player->m_Transform.translation.x - m_Transform.translation.x < 0.01f && isFaceRight){
        m_Transform.scale.x = -std::abs(m_Transform.scale.x);
        isFaceRight = false;
    }
}