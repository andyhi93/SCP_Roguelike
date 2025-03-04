#include "SCP610.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP610::SCP610() : Enemy(){
	std::random_device rd;  // 隨機數種子
	std::mt19937 gen(rd()); // 使用 Mersenne Twister PRNG
	std::uniform_real_distribution<float> dis(0.0f, 5.0f); // 生成 0 到 5 之間的 float

	m_LastShotTime = dis(gen);

	health = 1;
	m_ShotInterval = 5;
	speed = 2.0f;
	//m_Transform.translation = { 50,0 };
	m_Collider = std::make_shared<Collider>(m_Transform.translation, glm::vec2{ 45,150 });
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP610/scp610_walk1.png", "../../../Resources/SCP610/scp610_walk2.png", 
		"../../../Resources/SCP610/scp610_walk3.png", "../../../Resources/SCP610/scp610_walk4.png", }, true, 100, true, 100);
	m_AnimationAttack = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP610/scp610_attack1.png", "../../../Resources/SCP610/scp610_attack2.png",
		"../../../Resources/SCP610/scp610_attack3.png", "../../../Resources/SCP610/scp610_attack4.png", }, true, 200, true, 200);
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/die_animation1.png", "../../../Resources/die_animation2.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationAttack->SetLooping(false);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 5,5 };
}
void SCP610::Behavior() {
	m_Collider->position = m_Transform.translation;

	if (!m_Collider->CheckCollision(*m_Collider, *m_Player->m_Collider)) {
		glm::vec2 direction = m_Player->m_Transform.translation - m_Transform.translation;
		glm::vec2 velocity;
		velocity = glm::normalize(direction);
		velocity = { velocity.x * speed,velocity.y * speed };
		m_Transform.translation += m_Collider->blockDetect(velocity);
	}

	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentTime - m_LastShotTime >= m_ShotInterval) {
		SetDrawable(m_AnimationAttack);
		m_AnimationAttack->SetCurrentFrame(0);
		m_AnimationAttack->Play();
		m_LastShotTime = currentTime;
	}
	if (m_AnimationAttack->GetCurrentFrameIndex() == 2 && !isFire) {
		glm::vec2 bulletDirection = glm::normalize(m_Player->m_Transform.translation - m_Transform.translation);
		auto bullet = std::make_shared<Bullet>(damage, CollisionLayer::Enemy, 10.0f, 1, bulletDirection);
		bullet->m_Transform.translation = m_Transform.translation;
		m_BulletBox->AddBullet(bullet);
		isFire = true;
	}
	if (m_AnimationAttack->GetCurrentFrameIndex() == 3) {
		SetDrawable(m_AnimationWalk);
		isFire = false;
	}
}
void SCP610::Update() {
	m_BulletBox->Update() ;	
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		isDead = true;
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}