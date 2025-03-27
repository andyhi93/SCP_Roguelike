#include "Enemy.hpp"
#include "Player.hpp"
#include "random"
#include "Item.hpp"
#include "BulletBox.hpp"

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

void Enemy::SetPlayer(std::weak_ptr<Player> _player) { 
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
    if (m_Player.lock()->m_Transform.translation.x - m_Transform.translation.x > 0.01f && !isFaceRight) {
        m_Transform.scale.x = std::abs(m_Transform.scale.x);
        isFaceRight = true;
    }
    else if(m_Player.lock()->m_Transform.translation.x - m_Transform.translation.x < 0.01f && isFaceRight){
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
        auto player = std::dynamic_pointer_cast<Player>(other->parentActor.lock());
        if (player && !player->isDashing) {
            player->Damage(MeleeDamage);
        }
    }
}
IRangedAttack::IRangedAttack(std::weak_ptr<Enemy> ownerEnemy,std::weak_ptr<Util::Animation> m_AnimationShoot,int ammoIndex) {
    this->ownerEnemy = ownerEnemy;
    this->m_AnimationShoot = m_AnimationShoot;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, shootSpeed-1);
    m_LastShootTime = dis(gen);
}
void IRangedAttack::Shoot() {
    currentTime = SDL_GetTicks() / 1000.0f;
    if (currentTime - m_LastShootTime >= shootSpeed) {
        m_LastShootTime = currentTime;
        isFire = false;
    }
    if (!isFire) {//Get Size tEST
        ownerEnemy.lock()->state = Enemy::State::Attack;
        m_SFX->LoadMedia(ownerEnemy.lock()->m_throwSFX);
        m_SFX->Play();
        isFire = true;
        isAnimDone = false;
        m_AnimationShoot.lock()->SetCurrentFrame(0);
        ownerEnemy.lock()->SetDrawable(m_AnimationShoot.lock());
        m_AnimationShoot.lock()->Play();
        glm::vec2 bulletDirection = ownerEnemy.lock()->m_Player.lock()->m_Transform.translation - ownerEnemy.lock()->m_Transform.translation;
        auto bullet = std::make_shared<Bullet>(ownerEnemy.lock()->m_Transform.translation, BulletDamage, CollisionLayer::Enemy, 10.0f, ammoIndex, bulletDirection);
        bullet->m_Transform.translation = ownerEnemy.lock()->m_Transform.translation;
        m_BulletBox->AddBullet(bullet);
    }
    if (!isAnimDone && m_AnimationShoot.lock()->GetCurrentFrameIndex()==m_AnimationShoot.lock()->GetFrameCount() - 1) {
        isAnimDone = true;
    }
}