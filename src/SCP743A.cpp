#include "SCP743A.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743A::SCP743A() : Enemy(glm::vec2{ 45,140 }) {
	std::random_device rd;  // 隨機數種子
	std::mt19937 gen(rd()); // 使用 Mersenne Twister PRNG
	std::uniform_real_distribution<float> dis(0.0f, 5.0f); // 生成 0 到 5 之間的 float

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());

	m_LastAttackTime = dis(gen);
	attackSpeed = 1;

	health = 1;
	speed = 5.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP743/SCP743A_walk1.png", "../../../Resources/SCP743/SCP743A_walk2.png", 
		"../../../Resources/SCP743/SCP743A_walk3.png", "../../../Resources/SCP743/SCP743A_walk4.png", 
		"../../../Resources/SCP743/SCP743A_walk5.png", }, true, 100, true, 100);
	m_AnimationAttack = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP743/SCP743A_walk1.png", "../../../Resources/SCP743/SCP743A_walk2.png",
		"../../../Resources/SCP743/SCP743A_walk3.png", "../../../Resources/SCP743/SCP743A_walk4.png",
		"../../../Resources/SCP743/SCP743A_walk5.png", }, true, 100, true, 100);
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP743/SCP743A_die.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationAttack->SetLooping(false);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP743A::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (other->tag == "Player") {
		if (currentTime - m_LastAttackTime >= attackSpeed) {
			if (m_collider->CheckCollision(m_Player->m_collider)) {
				m_Player->health -= damage;
			}
			m_LastAttackTime = currentTime;
		}
	}
}
void SCP743A::Behavior() {
	glm::vec2 direction = m_Player->m_Transform.translation - m_Transform.translation;
	direction = { direction.x / sqrt(direction.x * direction.x + direction.y * direction.y),direction.y / sqrt(direction.x * direction.x + direction.y * direction.y) };
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
}
void SCP743A::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		isDead = true;
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}