#include "SCP553.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP553::SCP553() : Enemy() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 2.0f);

	attackSpeed = 3;
	m_LastAttackTime = dis(gen);

	health = 1;
	speed = 3.0f;
	//m_Transform.translation = { 50,0 };
	m_Collider = std::make_shared<Collider>(m_Transform.translation, glm::vec2{ 45,150 });
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
void SCP553::Behavior() {
	m_Collider->position = m_Transform.translation - glm::vec2{ m_Collider->size[0] / 2, m_Collider->size[1] / 2 };

	if (!m_Collider->CheckCollision(*m_Collider, *m_Player->m_Collider)) {
		glm::vec2 direction = m_Player->m_Transform.translation - m_Transform.translation;
		glm::vec2 velocity;
		velocity = glm::normalize(direction);
		velocity = { velocity.x * speed,velocity.y * speed };
		m_Transform.translation += m_Collider->blockDetect(velocity);
	}

	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentTime - m_LastAttackTime >= attackSpeed) {
		if (m_Collider->CheckCollision(*m_Collider, *m_Player->m_Collider)) {
			m_Player->health -= damage;
		}
		m_LastAttackTime = currentTime;
	}
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