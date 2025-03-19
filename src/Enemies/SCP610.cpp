#include "Enemies/SCP610.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP610::SCP610() : Enemy(glm::vec2{ 30,80 }){

	m_collider->offset = { 0,-10 };
	isDropCoin = true;

	health = 1;
	speed = 2.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP610/scp610_walk1.png", RESOURCE_DIR "/SCP610/scp610_walk2.png",
		RESOURCE_DIR "/SCP610/scp610_walk3.png", RESOURCE_DIR "/SCP610/scp610_walk4.png", }, true, 100, true, 100);
	m_AnimationAttack = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP610/scp610_attack1.png", RESOURCE_DIR "/SCP610/scp610_attack2.png",
		RESOURCE_DIR "/SCP610/scp610_attack3.png", RESOURCE_DIR "/SCP610/scp610_attack4.png", }, true, 200, true, 200);
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationAttack->SetLooping(false);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 5,5 };
}
void SCP610::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP610::SetActive(bool isActive) {
	m_collider->isActive = isActive;
	if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
	if (!isActive && m_IRangedAttack) m_IRangedAttack->m_BulletBox->ChangeRoom();
}
void SCP610::Behavior() {
	glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
	Shootable();
}
void SCP610::Start() {
	m_collider->parentActor = shared_from_this();
	m_IRangedAttack=std::make_shared<IRangedAttack>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), m_AnimationAttack,1);
	this->AddChild(m_IRangedAttack->m_BulletBox);
}
void SCP610::FixedUpdate() {
	if(m_IRangedAttack) m_IRangedAttack->m_BulletBox->FixedUpdate();
}
void SCP610::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		SetDead();
		SetActive(false);
	}
	if (!isDead) {
		FlipControl();
		Behavior();
		if (state!=State::Walk &&m_IRangedAttack->isAnimDone) {
			SetDrawable(m_AnimationWalk);
			state = State::Walk;
		}
	}
}
void SCP610::Shootable() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentTime - m_IRangedAttack->m_LastShootTime >= m_IRangedAttack->shootSpeed) {
		m_IRangedAttack->m_LastShootTime = currentTime;
		m_IRangedAttack->isFire = false;
	}
	if (!m_IRangedAttack->isFire) {//Get Size tEST
		state = Enemy::State::Attack;
		m_IRangedAttack->isFire = true;
		m_IRangedAttack->isAnimDone = false;
		m_IRangedAttack->m_AnimationShoot.lock()->SetCurrentFrame(0);
		SetDrawable(m_IRangedAttack->m_AnimationShoot.lock());
		m_IRangedAttack->m_AnimationShoot.lock()->Play();
		glm::vec2 bulletDirection = m_Player.lock()->m_Transform.translation - m_Transform.translation;
		auto bullet = std::make_shared<Bullet>(m_Transform.translation, m_IRangedAttack->BulletDamage, CollisionLayer::Enemy, 10.0f, m_IRangedAttack->ammoIndex, bulletDirection);
		bullet->m_Transform.translation = m_Transform.translation;
		m_IRangedAttack->m_BulletBox->AddBullet(bullet);
	}
	if (!m_IRangedAttack->isAnimDone && m_IRangedAttack->m_AnimationShoot.lock()->GetCurrentFrameIndex() == m_IRangedAttack->m_AnimationShoot.lock()->GetFrameCount() - 1) {
		m_IRangedAttack->isAnimDone = true;
	}
}