#include "Enemies/SCP049.hpp"
#include "Enemies/SCP049_2.hpp"
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
		std::vector<std::string>{RESOURCE_DIR "/SCP049/SCP049Die.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };

	HealthBackground->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/BossHealthBackground.png"));
	HealthBackground->m_Transform.translation={ 0,-450 };
	HealthBackground->m_Transform.scale = { 3,3 };
	HealthBackground->SetZIndex(10);
	this->AddChild(HealthBackground);

	HealthBarImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/BossHealthBar.png"));
	HealthBarImage->m_Transform.translation = { -360,-450 };
	HealthBarImage->m_Transform.scale = { 3,3 };
	HealthBarImage->SetZIndex(10.2f);
	this->AddChild(HealthBarImage);

	HealthNameImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/SCP049UI.png"));
	HealthNameImage->m_Transform.translation = { 0,-495 };
	HealthNameImage->m_Transform.scale = { 3,3 };
	HealthNameImage->SetZIndex(10.3f);
	this->AddChild(HealthNameImage);
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
	Shootable();

	for (int i = 2; i >-1; --i) {
		if (HealthQuarter[i] == 1) {
			auto targetDiff = targetPoint - m_WorldCoord;
			if (abs(targetDiff.x) > 180 || abs(targetDiff.y) > 180) {
				glm::vec2 direction = normalize(targetDiff);
				currentMoveState = MoveState::Patrol;
				if (abs(targetDiff.x) > 180) {
					MoveX(direction.x * speed*3);
				}
				if (abs(targetDiff.y) > 180) {
					MoveY(direction.y * speed*3);
				}
				return;
			}
			else {
				HealthQuarter[i] = 2;
				currentMoveState = MoveState::Idle;
			}
		}
		if (HealthQuarter[i] == 0 && health <= (maxHealth / 4 * (i + 1))) {
			HealthQuarter[i] = 1;
			glm::vec2 target= patrolPoint[0];
			for (auto& point : patrolPoint) {
				auto diff = point - m_WorldCoord;
				auto maxDiff = target - m_WorldCoord;
				if ((abs(diff.x) + abs(diff.y)) > (abs(maxDiff.x) + abs(maxDiff.y))) target = point;
			}
			targetPoint = target;
			break;
		}
	}

	auto playerDiff = m_Player.lock()->m_Transform.translation - m_Transform.translation;
	if (abs(playerDiff.x) > 180 || abs(playerDiff.y) > 180) {
		glm::vec2 direction = normalize(playerDiff);
		currentMoveState = MoveState::Chase;
		if (abs(playerDiff.x) > 180) {
			MoveX(direction.x * speed);
		}
		if (abs(playerDiff.y) > 180) {
			MoveY(direction.y * speed);
		}
	}
	else {
		MoveX(0);
		MoveY(0);
		currentMoveState = MoveState::Idle;
	}
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
		HealthBackground->SetVisible(false);
		HealthBarImage->SetVisible(false);
		HealthNameImage->SetVisible(false);
	}
	if (!isDead) {
		HealthBarImage->m_Transform.scale = { 3 * health / maxHealth,3 };
		FlipControl();
		Behavior();
		if (currentMoveState != Idle) {
			SetDrawable(m_AnimationWalk);
		}
		else{ SetDrawable(m_AnimationIdle); }
	}
}
void SCP049::Shootable() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentAttackState==StopShoot && !isSummon && currentTime - m_LastSummonTime >= summonColdDown) {
		currentAttackState = Summon;
		m_LastSummonTime = currentTime;
		isSummon = true;
	}
	else if (currentAttackState == StopShoot && currentTime - m_LastAttackTime >= 3) {
		currentAttackState = Shoot;
		m_FirstAttackTIme = currentTime;
		shootIndex = (shootIndex) ? 0 : 1;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(5, 10);
		m_attackTime = dis(gen);
	}
	if (currentAttackState == Shoot) {
		m_LastAttackTime = currentTime;
		if (currentTime - m_IRangedAttack->m_LastShootTime >= m_IRangedAttack->shootSpeed) {
			m_IRangedAttack->m_LastShootTime = currentTime;
			m_IRangedAttack->isFire = false;
		}
		if (!m_IRangedAttack->isFire) {
			std::vector<glm::vec2> bulletDirection;
			int angle=270;

			bulletDirection = { {0,1},{0.5,1},{1,0.5},{1,0},{1,-0.5},{0.5,-1},{0,-1},{-0.5,-1},{-1,-0.5},{-1,0},{-1,0.5},{-0.5,1} };
			/*if (shootIndex) {
				//bulletDirection = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} }; 
				angle = 270;
			}
			else { 
				bulletDirection = { {0.5,1},{1,0.5},{1,-0.5},{0.5,-1},{-0.5,-1},{-1,-0.5},{-1,0.5},{-0.5,1} }; 
				angle = 248;
			}*/
			for (int i = 0; i < 12; i++) {
				std::shared_ptr<Bullet> bullet;
				if(health > maxHealth /2){
					bullet = std::make_shared<Bullet>(m_Transform.translation + glm::vec2{ bulletDirection[i].x * 5, bulletDirection[i].y * 5 }, damage, CollisionLayer::Enemy, 15.0f, 3, bulletDirection[i]);
					bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ bulletDirection[i].x * 5, bulletDirection[i].y * 5 };
				}
				else {
					bullet = std::make_shared<Bullet>(m_Transform.translation + glm::vec2{ 50 * (isFaceRight ? -1 : 1),0} + glm::vec2{ bulletDirection[i].x * -150, bulletDirection[i].y * -150 }, damage, CollisionLayer::Enemy, 25, 3, bulletDirection[i]);
					bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ 50*(isFaceRight?-1:1),0} + glm::vec2{bulletDirection[i].x * -150, bulletDirection[i].y * -150};
				}
				
				bullet->m_collider->size = { 10,10 };
				bullet->m_Transform.scale = { 2.5f,2.5f };
				bullet->m_Transform.rotation = (angle - i * 30) * (M_PI / 180.0);
				m_IRangedAttack->m_BulletBox->AddBullet(bullet);
			}
			m_IRangedAttack->isFire = true;
		}
		if (currentTime - m_FirstAttackTIme >= m_attackTime) {
			currentAttackState = StopShoot;
		}
	}
}
std::vector<std::shared_ptr<Object>> SCP049::summon() {
	isSummon = false;
	currentAttackState = StopShoot;
	auto scp049_2 = std::make_shared<SCP049_2>();
	this->AddChild(scp049_2);
	scp049_2->Start();
	scp049_2->isCameraOn = true;
	scp049_2->SetPlayer(m_Player);
	scp049_2->SetZIndex(GetZIndex() - 0.01f);
	scp049_2->m_collider->position = m_Transform.translation + glm::vec2{ (isFaceRight) ? 120 : -120,0 };
	scp049_2->m_Transform.translation = m_Transform.translation + glm::vec2{ (isFaceRight) ? 120 : -120,0 };
	scp049_2->m_WorldCoord=m_WorldCoord+ glm::vec2{ (isFaceRight) ? 120 : -120,0 };
	return { scp049_2 };
}