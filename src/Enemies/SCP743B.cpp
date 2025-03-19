#include "Enemies/SCP743B.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743B::SCP743B() : Enemy(glm::vec2{ 45,45 }) {
	canFly = true;
	isDropCoin = true;

	health = 1;
	speed = 2.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743B_fly1.png", RESOURCE_DIR "/SCP743/SCP743B_fly2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743B_die1.png", RESOURCE_DIR "/SCP743/SCP743B_die2.png",
		RESOURCE_DIR "/SCP743/SCP743B_die3.png", RESOURCE_DIR "/SCP743/SCP743B_die4.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 2.5f,2.5f };
}
void SCP743B::Start() {
	m_collider->parentActor = shared_from_this();
	m_IRangedAttack = std::make_shared<IRangedAttack>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), m_AnimationAttack,2);
	this->AddChild(m_IRangedAttack->m_BulletBox);
	m_IRangedAttack->shootSpeed = 3.0f;
}
void SCP743B::SetActive(bool isActive) {
	m_collider->isActive = isActive;
	if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
	if (!isActive && m_IRangedAttack) m_IRangedAttack->m_BulletBox->ChangeRoom();
}
void SCP743B::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP743B::Behavior() {
	glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - this->m_Transform.translation);
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
	m_IRangedAttack->Shoot();
}
void SCP743B::FixedUpdate() {
	m_IRangedAttack->m_BulletBox->FixedUpdate();
}
void SCP743B::Update() {
	m_IRangedAttack->m_BulletBox->Update();
	if (health <= 0 && !isDead) {
		SetDrawable(m_AnimationDie);
		SetDead();
		SetActive(false);
	}
	if (!isDead) {
		FlipControl();
		Behavior();
		if (state != State::Walk && m_IRangedAttack->isAnimDone) {
			SetDrawable(m_AnimationWalk);
			state = State::Walk;
		}
	}
}