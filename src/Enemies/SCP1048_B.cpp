#include "Enemies/SCP1048_B.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP1048_B::SCP1048_B() : Enemy(glm::vec2{ 47,76 }) {
	isDropCoin = true;
	canFly = true;

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());

	health = 3;
	speed = 7.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP1048/SCP1048_B_walk1.png", RESOURCE_DIR "/SCP1048/SCP1048_B_walk2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP1048_B::SetActive(bool isActive) {
	m_collider->isActive = isActive;
	if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
	if (!isActive && m_IRangedAttack) m_IRangedAttack->m_BulletBox->ChangeRoom();
}
void SCP1048_B::Start() {
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

	m_IRangedAttack = std::make_shared<IRangedAttack>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), m_AnimationAttack,1);
	this->AddChild(m_IRangedAttack->m_BulletBox);
}
void SCP1048_B::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP1048_B::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player") {
			m_Player.lock()->Damage(damage);
	}
}
void SCP1048_B::Behavior() {
	m_meleeTrigger->FlipTrigger();

	glm::vec2 direction = patrolPos[targetIndex] - m_Transform.translation;
	glm::vec2 velocity;
	velocity = glm::normalize(direction);
	velocity = { velocity.x * speed,velocity.y * speed };
	MoveX(velocity.x);
	MoveY(velocity.y);
	if(abs(m_Transform.translation.x - patrolPos[targetIndex].x)<5 && abs(m_Transform.translation.y - patrolPos[targetIndex].y) < 5) {
		targetIndex = targetIndex > 2 ? 0 : targetIndex + 1;
	}
	m_IRangedAttack->Shoot();
}
void SCP1048_B::FixedUpdate() {
	m_IRangedAttack->m_BulletBox->FixedUpdate();
}
void SCP1048_B::Update() {
	m_IRangedAttack->m_BulletBox->Update();
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
		if (state != State::Walk && m_IRangedAttack->isAnimDone) {
			SetDrawable(m_AnimationWalk);
			state = State::Walk;
		}
	}
}