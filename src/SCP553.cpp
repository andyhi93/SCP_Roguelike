#include "SCP553.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP553::SCP553() : Enemy(glm::vec2{ 45,150 }) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 2.0f);

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());

	attackSpeed = 3;
	m_LastAttackTime = dis(gen);

	health = 1;
	speed = 3.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP553/SCP553_fly1.png", "../../../Resources/SCP553/SCP553_fly2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP553/SCP553_die1.png", "../../../Resources/SCP553/SCP553_die2.png",
		"../../../Resources/SCP553/SCP553_die3.png", "../../../Resources/SCP553/SCP553_die4.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 3,3 };
}
void SCP553::SetPlayer(std::shared_ptr<Player> _player) {
	m_Player = _player;
}
void SCP553::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (other->tag == "Player") {
		if (currentTime - m_LastAttackTime >= attackSpeed) {
			m_Player->Damage(damage);
			m_LastAttackTime = currentTime;
		}
	}
}
void SCP553::Behavior() {
	glm::vec2 direction = normalize(m_Player->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x*speed);
	MoveY(direction.y* speed);
}
void SCP553::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		isDead = true;
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}