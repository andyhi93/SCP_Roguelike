#include "Enemy.hpp"
#include "Player.hpp"

void Enemy::GetPlayer(std::shared_ptr<Player> _player) { m_Player = _player; }

void Enemy::FlipControl() {
    glm::vec2 PlayerPos = m_Player->m_Transform.translation;
    glm::vec2 direction = PlayerPos - m_Transform.translation;
    if (direction.x > 0 && !isFaceRight) {
        m_Transform.scale.x = std::abs(m_Transform.scale.x);
        isFaceRight = true;
    }
    else if(direction.x <= 0 && isFaceRight){
        m_Transform.scale.x = -std::abs(m_Transform.scale.x);
        isFaceRight = false;
    }
}