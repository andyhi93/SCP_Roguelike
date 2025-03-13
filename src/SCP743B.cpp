#include "SCP743B.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743B::SCP743B() : Enemy(glm::vec2{ 45,150 }) {
	isDropCoin = true;

	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<float> dis(0.0f, 2.0f); 

	attackSpeedUp = 3;
	m_LastAttackTime = dis(gen);
	m_BulletBox = std::make_shared<BulletBox>();
	this->AddChild(m_BulletBox);

	health = 1;
	speed = 2.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743B_fly1.png", RESOURCE_DIR "/SCP743/SCP743B_fly2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743B_die1.png", RESOURCE_DIR "/SCP743/SCP743B_die2.png",
		RESOURCE_DIR "/SCP743/SCP743B_die3.png", RESOURCE_DIR "/SCP743/SCP743B_die4.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 3,3 };
}
void SCP743B::SetPlayer(std::shared_ptr<Player> _player) {
	m_Player = _player;
	this->AddChild(m_BulletBox);
}
void SCP743B::Behavior() {
	glm::vec2 direction = normalize(m_Player->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
	Shoot();
}
void SCP743B::Shoot() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentTime - m_LastAttackTime >= attackSpeedUp) {
		SetDrawable(m_AnimationAttack);
		m_AnimationAttack->SetCurrentFrame(0);
		m_AnimationAttack->Play();
		m_LastAttackTime = currentTime;
		isFire = false;
	}
	if (m_AnimationAttack->GetCurrentFrameIndex() == 1 && !isFire) {
		glm::vec2 bulletDirection = m_Player->m_Transform.translation - m_Transform.translation;
		auto bullet = std::make_shared<Bullet>(m_Transform.translation, damage, CollisionLayer::Enemy, 10.0f, 2, bulletDirection);
		bullet->m_Transform.translation = m_Transform.translation;
		m_BulletBox->AddBullet(bullet);
		isFire = true;
		SetDrawable(m_AnimationWalk);
	}
}
void SCP743B::FixedUpdate() {
	m_BulletBox->FixedUpdate();
}
void SCP743B::Update() {
	m_BulletBox->Update();
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		SetDead();
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}