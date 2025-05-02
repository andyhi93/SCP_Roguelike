#include "Enemies/SCP3199.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP3199::SCP3199() : Enemy(glm::vec2{ 80,150 }) {
	isDropCoin = true;

	health = adultHealth;
	speed = 4.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/SCP3199_walk1.png", RESOURCE_DIR "/SCP3199/SCP3199_walk2.png", }, true, 100, true, 100);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP3199/SCP3199_die1.png", RESOURCE_DIR "/SCP3199/SCP3199_die2.png"
		, RESOURCE_DIR "/SCP3199/SCP3199_die3.png", RESOURCE_DIR "/SCP3199/SCP3199_die4.png"
		, RESOURCE_DIR "/SCP3199/SCP3199_die5.png", RESOURCE_DIR "/SCP3199/SCP3199_die6.png",
		RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 250, false,250);
	SetDrawable(m_AnimationWalk);
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

	if (currentState == walk && health <= 0) {
		SetDrawable(m_AnimationDie);
		currentState = die;
		speed = 0;
	}
	else if (currentState == die && m_AnimationDie->GetCurrentFrameIndex() == 7) {
		m_AnimationDie->SetCurrentFrame(0);
		m_meleeTrigger->m_collider->isActive = false;
		currentState = eggIdle;
		m_collider->size = glm::vec2{ 50,50 };
		m_collider->offset = glm::vec2{ 0,-50 };
		m_collider->position = m_Transform.translation + m_collider->offset;
		SetDrawable(m_AnimationEggIdle);
		health = childHealth;
		startSpawnTime= SDL_GetTicks() / 1000.0f;
	}
	else if (currentState == eggIdle) {
		if (health <= 0) {
			currentState = childDie;
			SetDrawable(m_AnimationEggBorn);
		}
		currentTime = SDL_GetTicks() / 1000.0f;
		if (currentTime - startSpawnTime >= 5) {
			m_collider->size = glm::vec2{ 50,80 };
			m_collider->offset = glm::vec2{ 0,0 };
			m_collider->position = m_Transform.translation + m_collider->offset;
			currentState = eggBorn;
			health = childHealth;
			SetDrawable(m_AnimationEggBorn);
			startGrowTime = currentTime;
		}
	}
	else if (currentState == eggBorn && m_AnimationEggBorn->GetCurrentFrameIndex()==3) {
		m_meleeTrigger->m_collider->isActive = true;
		m_meleeTrigger->m_collider->size = m_collider->size + glm::vec2{ 20,20 };
		currentState = childWalk;
		SetDrawable(m_AnimationChildWalk);
		health = childHealth;
		speed = 6;
	}
	else if (currentState == childWalk) {
		if (health <= 0) {
			currentState = childDie;
			SetDrawable(m_AnimationChildDie);
		}
		currentTime = SDL_GetTicks() / 1000.0f;
		if (currentTime - startGrowTime >= 5) {
			speed = 4;
			currentState = walk;
			SetDrawable(m_AnimationWalk);
			health = adultHealth;
			m_collider->size = glm::vec2{ 80,150 };
			m_meleeTrigger->m_collider->size = m_collider->size+ glm::vec2{ 20,20 };
		}
	}

	if (!m_Player.lock()->isDead) {
		glm::vec2 direction = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);
		MoveX(direction.x * speed);
		MoveY(direction.y * speed);
	}
}
void SCP3199::Update() {
	if (currentState==childDie) {
		m_meleeTrigger->m_collider->isActive = false;
		m_collider->isActive = false;
		SetDead();
		SetActive(false);
	}
	if (!isDead) {
		FlipControl();
		Behavior();
	}
}