#include "Enemies/SCP049_2.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP049_2::SCP049_2() : Enemy(glm::vec2{ 30,140 }){
	m_collider->offset = { 0,0 };
	isDropCoin = true;

	std::random_device rd;  // 隨機數種子
	std::mt19937 gen(rd()); // 使用 Mersenne Twister PRNG
	std::uniform_real_distribution<float> dis(0.0f, 5.0f); // 生成 0 到 5 之間的 float

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());


	health = 5;
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
void SCP049_2::Start() {
	m_collider->parentActor = shared_from_this();
	if (isDropCoin) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 4);
		int cmd = dis(gen);
		if (cmd == 0 || cmd == 1 || cmd == 2) isDropCoin = false;
	}
	m_meleeTrigger = std::make_shared<IMeleeTrigger>(m_collider->size);
	m_meleeTrigger->ownerEnemy = std::dynamic_pointer_cast<Enemy>(shared_from_this());  // 設定 ownerEnemy
	m_meleeTrigger->m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(m_meleeTrigger));
}
void SCP049_2::Behavior() {
	if (!m_Player.lock()->isDead) {
		glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
		MoveX(direction.x * speed);
		MoveY(direction.y * speed);
	}
	//MeleeAttack
	m_meleeTrigger->FlipTrigger();
}
void SCP049_2::Update() {
	if (health <= 0 && !isDead) {
		m_meleeTrigger->m_collider->isActive = false;
		SetDrawable(m_AnimationDie);
		SetDead();
		PlayDeadSE(dead_SEPath);
		SetActive(false);
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}