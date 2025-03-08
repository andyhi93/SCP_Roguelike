#include "SCP049_2.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP049_2::SCP049_2() : Enemy(glm::vec2{ 45,140 }) {
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
		std::vector<std::string>{"../../../Resources/SCP049_2/SCP049_2_walk1.png", "../../../Resources/SCP049_2/SCP049_2_walk2.png",
		"../../../Resources/SCP049_2/SCP049_2_walk3.png", "../../../Resources/SCP049_2/SCP049_2_walk4.png",
		"../../../Resources/SCP049_2/SCP049_2_walk5.png", "../../../Resources/SCP049_2/SCP049_2_walk6.png", 
		"../../../Resources/SCP049_2/SCP049_2_walk7.png", "../../../Resources/SCP049_2/SCP049_2_walk8.png", 
		"../../../Resources/SCP049_2/SCP049_2_walk9.png", "../../../Resources/SCP049_2/SCP049_2_walk10.png", }, true, 100, true, 100);
	m_AnimationAttack = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP049_2/SCP049_2_walk1.png", "../../../Resources/SCP049_2/SCP049_2_walk2.png",
		"../../../Resources/SCP049_2/SCP049_2_walk3.png", "../../../Resources/SCP049_2/SCP049_2_walk4.png",
		"../../../Resources/SCP049_2/SCP049_2_walk5.png", "../../../Resources/SCP049_2/SCP049_2_walk6.png",
		"../../../Resources/SCP049_2/SCP049_2_walk7.png", "../../../Resources/SCP049_2/SCP049_2_walk8.png",
		"../../../Resources/SCP049_2/SCP049_2_walk9.png", "../../../Resources/SCP049_2/SCP049_2_walk10.png", }, true, 100, true, 100);
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/die_animation1.png", "../../../Resources/die_animation2.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationAttack->SetLooping(false);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP049_2::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
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
void SCP049_2::Behavior() {
	glm::vec2 direction = m_Player->m_Transform.translation - m_Transform.translation;
	direction = { direction.x / sqrt(direction.x * direction.x + direction.y * direction.y),direction.y / sqrt(direction.x * direction.x + direction.y * direction.y) };
	MoveX(direction.x* speed);
	MoveY(direction.y* speed);
}
void SCP049_2::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		isDead = true;
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}