#include "Enemies/SCP3199.hpp"
#include "Enemies/SCP3199_2.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP3199::SCP3199() : Enemy(glm::vec2{ 80,150 }) {
	isDropCoin = true;

	health = 7;
	speed = 4.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/SCP3199_walk1.png", RESOURCE_DIR "/SCP3199/SCP3199_walk2.png", }, true, 100, true, 100);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/SCP3199_die1.png", RESOURCE_DIR "/SCP3199/SCP3199_die2.png"
		, RESOURCE_DIR "/SCP3199/SCP3199_die3.png", RESOURCE_DIR "/SCP3199/SCP3199_die4.png"
		, RESOURCE_DIR "/SCP3199/SCP3199_die5.png", RESOURCE_DIR "/SCP3199/SCP3199_die6.png",
		RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 3,3 };
}
void SCP3199::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player") {
		m_Player.lock()->Damage(damage);
	}
}
void SCP3199::Start() {
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
void SCP3199::Behavior() {
	m_meleeTrigger->FlipTrigger();

	if (!m_Player.lock()->isDead) {
		glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
		MoveX(direction.x * speed);
		MoveY(direction.y * speed);
	}
}
void SCP3199::Update() {
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
std::vector<std::shared_ptr<Object>> SCP3199::summon() {
	isSummon = false;
	auto scp3199_2 = std::make_shared<SCP3199_2>();
	this->AddChild(scp3199_2);
	scp3199_2->Start();
	scp3199_2->SetPlayer(m_Player);
	scp3199_2->SetZIndex(GetZIndex() + 0.01f);
	scp3199_2->m_collider->position = m_Transform.translation;
	scp3199_2->m_Transform.translation = m_Transform.translation;
	return { scp3199_2 };
}