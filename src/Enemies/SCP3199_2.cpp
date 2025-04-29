#include "Enemies/SCP3199_2.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP3199_2::SCP3199_2() : Enemy(glm::vec2{ 50,80 }) {
	isDropCoin = true;

	health = 6;
	speed = 4.0f;
	m_AnimationIdle = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/egg_idle1.png", RESOURCE_DIR "/SCP3199/egg_idle2.png", }, true, 500, true, 500);
	m_AnimationBorn = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/egg_crack1.png", RESOURCE_DIR "/SCP3199/egg_crack2.png", 
		RESOURCE_DIR "/SCP3199/egg_crack3.png", RESOURCE_DIR "/SCP3199/egg_crack4.png", }, true, 500, true, 500);
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/SCP3199_2_walk1.png", RESOURCE_DIR "/SCP3199/SCP3199_2_walk2.png", }, true, 100, true, 100);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{
		RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationIdle);
	m_AnimationDie->SetLooping(false);
	m_AnimationBorn->SetLooping(false);
	m_AnimationIdle->Play();
	m_Transform.scale = { 3,3 };
	startSpawnTime= SDL_GetTicks() / 1000.0f;
}
void SCP3199_2::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player") {
		m_Player.lock()->Damage(damage);
	}
}
void SCP3199_2::Start() {
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
	m_meleeTrigger->m_collider->isActive = false;
}
void SCP3199_2::Behavior() {
	if (currentState == eggIdle) {
		currentTime = SDL_GetTicks() / 1000.0f;
		if (currentTime- startSpawnTime >= 5) {
			currentState = eggBorn;
			health = 5;
			SetDrawable(m_AnimationBorn);
		}
	}
	else if (currentState == eggBorn) {
		if (m_AnimationBorn->GetCurrentFrameIndex() == 3) {
			m_meleeTrigger->m_collider->isActive = true;
			currentState = walk;
			SetDrawable(m_AnimationWalk);
		}
	}
	else if (currentState == walk) {
		if (!m_Player.lock()->isDead) {
			glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
			MoveX(direction.x * speed);
			MoveY(direction.y * speed);
		}
		m_meleeTrigger->FlipTrigger();
	}
	else {
		m_meleeTrigger->m_collider->isActive = false;
	}
}
void SCP3199_2::Update() {
	if (health <= 0 && !isDead) {
		if (currentState == eggBorn) {
			SetDrawable(m_AnimationBorn);
		}
		else {
			SetDrawable(m_AnimationDie);
		}
		currentState = die;
		SetDead();
		SetActive(false);
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}