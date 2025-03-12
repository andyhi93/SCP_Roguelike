#include "SCP049_2.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP049_2::SCP049_2() : Enemy(glm::vec2{ 45,140 }) {
	isDropCoin = true;

	std::random_device rd;  // 隨機數種子
	std::mt19937 gen(rd()); // 使用 Mersenne Twister PRNG
	std::uniform_real_distribution<float> dis(0.0f, 5.0f); // 生成 0 到 5 之間的 float

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());

	m_LastAttackTime = dis(gen);
	attackSpeed = 1;

	health = 1;
	speed = 2.0f;
	//m_Transform.translation = { 50,0 };
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP049_2/SCP049_2_walk1.png", RESOURCE_DIR "/SCP049_2/SCP049_2_walk2.png",
		RESOURCE_DIR "/SCP049_2/SCP049_2_walk3.png", RESOURCE_DIR "/SCP049_2/SCP049_2_walk4.png",
		RESOURCE_DIR "/SCP049_2/SCP049_2_walk5.png", RESOURCE_DIR "/SCP049_2/SCP049_2_walk6.png",
		RESOURCE_DIR "/SCP049_2/SCP049_2_walk7.png", RESOURCE_DIR "/SCP049_2/SCP049_2_walk8.png",
		RESOURCE_DIR "/SCP049_2/SCP049_2_walk9.png", RESOURCE_DIR "/SCP049_2/SCP049_2_walk10.png", }, true, 100, true, 100);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP049_2::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (other->tag == "Player") {
		if (currentTime - m_LastAttackTime >= attackSpeed) {
			m_Player->Damage(damage);
			m_LastAttackTime = currentTime;
		}
	}
}
void SCP049_2::Behavior() {
	glm::vec2 direction = normalize(m_Player->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x* speed);
	MoveY(direction.y* speed);
}
void SCP049_2::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		SetDead();
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}