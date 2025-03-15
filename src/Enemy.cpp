#include "Enemy.hpp"
#include "Player.hpp"
#include "random"
#include "Item.hpp"

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
void Enemy::SetActive(bool isActive) {
    m_collider->isActive = isActive;
    if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
}

void Enemy::SetPlayer(std::shared_ptr<Player> _player) { 
    m_Player = _player; 
}
std::shared_ptr<Item> Enemy::GetCoin() {
    auto coin = std::make_shared<Item>(m_Transform.translation,Item::bloodCoin);
    coin->SetItemType(Item::bloodCoin);
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
IMeleeTrigger::IMeleeTrigger(glm::vec2 size) :Actor(size) {
    m_collider->isTrigger = true;
    m_collider->tag = "Melee";
    /*
	m_meleeTrigger = std::make_shared<IMeleeTrigger>(m_collider->size);
	m_meleeTrigger->ownerEnemy = std::dynamic_pointer_cast<Enemy>(shared_from_this());  // 設定 ownerEnemy
	m_meleeTrigger->m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(m_meleeTrigger));

    //MeleeAttack Update
    m_meleeTrigger->FlipTrigger();
    */
}
void IMeleeTrigger::FlipTrigger() {
    auto enemy = ownerEnemy.lock() ;    
    if (enemy) {  // 確保 Enemy 仍然有效
        if (enemy->GetIsFacingRight()) {
            m_collider->position = enemy->m_Transform.translation + glm::vec2{ m_collider->size.x/2, 0};
        }
        else {
            m_collider->position = enemy->m_Transform.translation - glm::vec2{ m_collider->size.x / 2, 0 };
        }
    }
}
void IMeleeTrigger::OnTriggerStay(std::shared_ptr<BoxCollider> other) {
    if (other->tag == "Player") {
        auto player = std::dynamic_pointer_cast<Player>(other->parentActor);
        if (player) {
            player->Damage(MeleeDamage);
        }
    }
}