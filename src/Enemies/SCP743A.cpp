#include "Enemies/SCP743A.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743A::SCP743A() : Enemy(glm::vec2{ 80,30 }) {
	isDropCoin = true;

	health = 3;
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
	if (other->tag == "Player") {
			m_Player.lock()->Damage(damage);
	}
}

void SCP743A::Start() {
	m_collider->parentActor = shared_from_this();
	if (isDropCoin) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 4);
		int cmd = dis(gen);
		if (cmd == 0 || cmd == 1 || cmd == 2) isDropCoin = false;
	}
	m_meleeTrigger = std::make_shared<IMeleeTrigger>(m_collider->size + glm::vec2{ 20,20 });
	m_meleeTrigger->ownerEnemy = std::dynamic_pointer_cast<Enemy>(shared_from_this());  // ³]©w ownerEnemy
	m_meleeTrigger->m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(m_meleeTrigger));
}
void SCP743A::Behavior() {
	m_meleeTrigger->FlipTrigger();

	if (!m_Player.lock()->isDead) {
		glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
		MoveX(direction.x * speed);
		MoveY(direction.y * speed);
	}
}
void SCP743A::Update() {
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