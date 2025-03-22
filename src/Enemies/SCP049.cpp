#include "Enemies/SCP049.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP049::SCP049() : Enemy(glm::vec2{ 50,200 }) {

	m_collider->offset = { 0,-10 };
	isDropCoin = true;

	health = 10;
	maxHealth = health;
	speed = 2.0f; 
	m_AnimationIdle = std::make_shared<Util::Animation>(
		std::vector<std::string>{
		RESOURCE_DIR "/SCP049/SCP049Idle1.png", RESOURCE_DIR "/SCP049/SCP049Idle2.png"}, true, 1000, true, 1000);
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{
		RESOURCE_DIR "/SCP049/SCP049Walk1.png", RESOURCE_DIR "/SCP049/SCP049Walk2.png", RESOURCE_DIR "/SCP049/SCP049Walk3.png", RESOURCE_DIR "/SCP049/SCP049Walk4.png"}, true, 300, true, 300);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP049/SCP049.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP049::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP049::SetActive(bool isActive) {
	m_collider->isActive = isActive;
	if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
	if (!isActive && m_IRangedAttack) m_IRangedAttack->m_BulletBox->ChangeRoom();
}
void SCP049::Behavior() {
	auto playerDiff = m_Player.lock()->m_Transform.translation - m_Transform.translation;
	if (abs(playerDiff.x) > 180 || abs(playerDiff.y) > 180) {
		glm::vec2 direction = normalize(playerDiff);
		currentState = State::Chase;
		SetDrawable(m_AnimationWalk);
		if (abs(playerDiff.x) > 180) {
			std::cout << "x far: " << abs(playerDiff.x) << "\n";
			MoveX(direction.x * speed);
		}
		if (abs(playerDiff.y) > 180) {
			std::cout << "y far: " << abs(playerDiff.y) << "\n";
			MoveY(direction.y * speed);
		}
	}
	else {
		SetDrawable(m_AnimationIdle);
		currentState = State::Idle;
	}
	Shootable();
}
void SCP049::Start() {
	m_collider->parentActor = shared_from_this();
	m_IRangedAttack = std::make_shared<IRangedAttack>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), m_AnimationAttack, 1);
	m_IRangedAttack->shootSpeed = 0.7f;
	this->AddChild(m_IRangedAttack->m_BulletBox);
}
void SCP049::FixedUpdate() {
	if (m_IRangedAttack) m_IRangedAttack->m_BulletBox->FixedUpdate();
}
void SCP049::Update() {
	m_IRangedAttack->m_BulletBox->Update();
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
void SCP049::Shootable() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentAttackState != StopShoot) {
		if (currentTime - m_IRangedAttack->m_LastShootTime >= m_IRangedAttack->shootSpeed) {
			m_IRangedAttack->m_LastShootTime = currentTime;
			m_IRangedAttack->isFire = false;
		}
		if (!m_IRangedAttack->isFire) {
			std::vector<glm::vec2> bulletDirection;
			int angle;
			if(shootIndex){ 
				bulletDirection = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} }; 
				angle = 270;
			}
			else { 
				bulletDirection = { {0.5,1},{1,0.5},{1,-0.5},{0.5,-1},{-0.5,-1},{-1,-0.5},{-1,0.5},{-0.5,1} }; 
				angle = 248;
			}
			for (int i = 0; i < 8; i++) {
				std::shared_ptr<Bullet> bullet;
				if(health > maxHealth /2){
					bullet = std::make_shared<Bullet>(m_Transform.translation + glm::vec2{ bulletDirection[i].x * 10, bulletDirection[i].y * 10 }, damage, CollisionLayer::Enemy, 10.0f, 3, bulletDirection[i]);
					bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ bulletDirection[i].x * 10, bulletDirection[i].y * 10 };
				}
				else {
					bullet = std::make_shared<Bullet>(m_Transform.translation + glm::vec2{100,0} + glm::vec2{ bulletDirection[i].x * -150, bulletDirection[i].y * -150 }, damage, CollisionLayer::Enemy, 15.0f, 3, bulletDirection[i]);
					bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ 100,0 } + glm::vec2{ bulletDirection[i].x * -150, bulletDirection[i].y * -150 };
				}
				
				bullet->m_collider->size = { 10,10 };
				bullet->m_Transform.scale = { 2.5f,2.5f };
				bullet->m_Transform.rotation = (angle - i * 45) * (M_PI / 180.0);
				m_IRangedAttack->m_BulletBox->AddBullet(bullet);
			}
			m_IRangedAttack->isFire = true;
		}
	}
}