#include "SCP743B.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743B::SCP743B() : Enemy() {
	std::random_device rd;  
	std::mt19937 gen(rd()); 
	std::uniform_real_distribution<float> dis(0.0f, 2.0f); 

	attackSpeed = 3;
	m_LastAttackTime = dis(gen);
	m_BulletBox = std::make_shared<BulletBox>();
	this->AddChild(m_BulletBox);

	health = 1;
	speed = 2.0f;
	//m_Transform.translation = { 50,0 };
	m_Collider = std::make_shared<Collider>(m_Transform.translation, glm::vec2{ 45,150 });
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP743/SCP743B_fly1.png", "../../../Resources/SCP743/SCP743B_fly2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP743/SCP743B_die1.png", "../../../Resources/SCP743/SCP743B_die2.png",
		"../../../Resources/SCP743/SCP743B_die3.png", "../../../Resources/SCP743/SCP743B_die4.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 3,3 };
}
void SCP743B::SetPlayer(std::shared_ptr<Player> _player) {
	m_Player = _player;
	m_BulletBox->setPlayer(m_Player);
	this->AddChild(m_BulletBox);
}
void SCP743B::Behavior() {
	m_Collider->position = m_Transform.translation - glm::vec2{ m_Collider->size[0] / 2, m_Collider->size[1] / 2 };

	if (!m_Collider->CheckCollision(*m_Collider, *m_Player->m_Collider)) {
		glm::vec2 direction = m_Player->m_Transform.translation - m_Transform.translation;
		glm::vec2 velocity;
		velocity = glm::normalize(direction);
		velocity = { velocity.x * speed,velocity.y * speed };
		m_Transform.translation += m_Collider->blockDetect(velocity);
	}
	Shoot();
}
void SCP743B::Shoot() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentTime - m_LastAttackTime >= attackSpeed) {
		SetDrawable(m_AnimationAttack);
		m_AnimationAttack->SetCurrentFrame(0);
		m_AnimationAttack->Play();
		m_LastAttackTime = currentTime;
		isFire = false;
	}
	if (m_AnimationAttack->GetCurrentFrameIndex() == 1 && !isFire) {
		glm::vec2 bulletDirection = m_Player->m_Transform.translation - m_Transform.translation;
		auto bullet = std::make_shared<Bullet>(damage, CollisionLayer::Enemy, 10.0f, 2, bulletDirection);
		bullet->m_Transform.translation = m_Transform.translation;
		m_BulletBox->AddBullet(bullet);
		isFire = true;
		SetDrawable(m_AnimationWalk);
	}
}
void SCP743B::Update() {
	m_BulletBox->Update();
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		isDead = true;
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}