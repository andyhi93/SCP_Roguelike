#include "SCP743A.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743A::SCP743A() : Enemy(glm::vec2{ 80,30 }) {
	isDropCoin = true;

	std::random_device rd;  // 隨機數種子
	std::mt19937 gen(rd()); // 使用 Mersenne Twister PRNG
	std::uniform_real_distribution<float> dis(0.0f, 5.0f); // 生成 0 到 5 之間的 float

	//m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));
	m_LastAttackTime = dis(gen);
	attackSpeed = 1;

	health = 1;
	speed = 5.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743A_walk1.png", RESOURCE_DIR "/SCP743/SCP743A_walk2.png",
		RESOURCE_DIR "/SCP743/SCP743A_walk3.png", RESOURCE_DIR "/SCP743/SCP743A_walk4.png",
		RESOURCE_DIR "/SCP743/SCP743A_walk5.png", }, true, 100, true, 100);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743A_die.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP743A::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (other->tag == "Player") {
		if (currentTime - m_LastAttackTime >= attackSpeed) {
			m_Player->Damage(damage);
			m_LastAttackTime = currentTime;
		}
	}
}
void SCP743A::Behavior() {
	glm::vec2 direction = normalize(m_Player->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
}
void SCP743A::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		SetDead();
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}