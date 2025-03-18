#include "SCP553.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP553::SCP553() : Enemy(glm::vec2{ 50,50 }) {
	canFly = true;

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());

	health = 2;
	speed = 3.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP553/SCP553_fly1.png", RESOURCE_DIR "/SCP553/SCP553_fly2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP553/SCP553_die1.png", RESOURCE_DIR "/SCP553/SCP553_die2.png",
		RESOURCE_DIR "/SCP553/SCP553_die3.png", RESOURCE_DIR "/SCP553/SCP553_die4.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 3,3 };
}
void SCP553::Start() {
	m_collider->parentActor = shared_from_this();
	if (isDropCoin) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 4);
		int cmd = dis(gen);
		if (cmd == 0 || cmd == 1 || cmd == 2) isDropCoin = false;
	}
	m_meleeTrigger = std::make_shared<IMeleeTrigger>(m_collider->size);
	m_meleeTrigger->ownerEnemy = std::dynamic_pointer_cast<Enemy>(shared_from_this());  // ³]©w ownerEnemy
	m_meleeTrigger->m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(m_meleeTrigger));
}
void SCP553::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP553::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player") {
			m_Player.lock()->Damage(damage);
	}
}
void SCP553::Behavior() {
	glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x*speed);
	MoveY(direction.y* speed);

	m_meleeTrigger->FlipTrigger();
}
void SCP553::Update() {
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		SetDead();
		SetActive(false);
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}