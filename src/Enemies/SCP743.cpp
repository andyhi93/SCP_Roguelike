#include "Enemies/SCP743.hpp"
#include "Enemies/SCP743A.hpp"
#include "Enemies/SCP743B.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP743::SCP743() : Enemy(glm::vec2{ 100,100 }) {

	m_collider->offset = { 0,-50 };
	isDropCoin = true;

	health = 30;
	maxHealth = health;
	speed = 2.0f;
	m_AnimationIdle = std::make_shared<Util::Animation>(
		std::vector<std::string>{
		RESOURCE_DIR "/SCP743/SCP743Idle1.png", RESOURCE_DIR "/SCP743/SCP743Idle2.png", RESOURCE_DIR "/SCP743/SCP743Idle3.png", RESOURCE_DIR "/SCP743/SCP743Idle4.png", }, true, 500, true, 500);
	m_AnimationWalk = m_AnimationIdle;
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP743/SCP743Die.png"}, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };

	HealthBackground->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/BossHealthBackground.png"));
	HealthBackground->m_Transform.translation = { 0,-450 };
	HealthBackground->m_Transform.scale = { 3,3 };
	HealthBackground->SetZIndex(10);
	this->AddChild(HealthBackground);

	HealthBarImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/BossHealthBar.png"));
	HealthBarImage->m_Transform.translation = { -360,-450 };
	HealthBarImage->m_Transform.scale = { 3,3 };
	HealthBarImage->SetZIndex(10.2f);
	this->AddChild(HealthBarImage);

	HealthNameImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/SCP743UI.png"));
	HealthNameImage->m_Transform.translation = { 0,-495 };
	HealthNameImage->m_Transform.scale = { 3,3 };
	HealthNameImage->SetZIndex(10.3f);
	this->AddChild(HealthNameImage);
}
void SCP743::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP743::SetActive(bool isActive) {
	m_collider->isActive = isActive;
	if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
	if (!isActive && m_IRangedAttack) m_IRangedAttack->m_BulletBox->ChangeRoom();
}
void SCP743::Behavior() {
	MoveX(0);
	MoveY(0);
	Shootable();
}
void SCP743::Start() {
	m_collider->parentActor = shared_from_this();
	m_IRangedAttack = std::make_shared<IRangedAttack>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), m_AnimationAttack, 1);
	m_IRangedAttack->shootSpeed = 0.7f;
	this->AddChild(m_IRangedAttack->m_BulletBox);
}
void SCP743::FixedUpdate() {
	if (m_IRangedAttack) m_IRangedAttack->m_BulletBox->FixedUpdate();
}
void SCP743::Update() {
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
		//FlipControl();
		Behavior();
	}
}
void SCP743::Shootable() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (currentAttackState == StopShoot && !isSummon && currentTime - m_LastSummonTime >= summonColdDown) {
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
			int angle = 250;

			bulletDirection = { {0,1},{0.5,1},{1,0.5},{1,0},{1,-0.5},{0.5,-1},{0,-1},{-0.5,-1},{-1,-0.5},{-1,0},{-1,0.5},{-0.5,1} };
			/*if (shootIndex) {
				//bulletDirection = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
				angle = 270;
			}
			else {
				bulletDirection = { {0.5,1},{1,0.5},{1,-0.5},{0.5,-1},{-0.5,-1},{-1,-0.5},{-1,0.5},{-0.5,1} };
				angle = 248;
			}*/
			if (health <= maxHealth / 2) ++secondIndex;
			if (health > maxHealth / 2) {
				for (int i = 0; i < 12; i++) {
					std::shared_ptr<Bullet> bullet;
					m_IRangedAttack->shootSpeed = 0.7f;
					bullet = std::make_shared<Bullet>(m_Transform.translation + glm::vec2{ bulletDirection[i].x * 5, bulletDirection[i].y * 5 }, damage, CollisionLayer::Enemy, 15.0f, 4, bulletDirection[i]);
					bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ bulletDirection[i].x * 5, bulletDirection[i].y * 5 };

					bullet->m_collider->size = { 10,10 };
					bullet->m_Transform.scale = { 2.5f,2.5f };
					bullet->m_Transform.rotation = (angle - i * 30) * (M_PI / 180.0);
					m_IRangedAttack->m_BulletBox->AddBullet(bullet);
				}
			}
			else {
				std::shared_ptr<Bullet> bullet;
				m_IRangedAttack->shootSpeed = 1.2f;
				bullet = std::make_shared<Bullet>(m_Transform.translation + glm::vec2{ 50 * (isFaceRight ? -1 : 1),0 } + glm::vec2{ bulletDirection[secondIndex].x * -150, bulletDirection[secondIndex].y * -150 }
					, damage, CollisionLayer::Enemy, 15, 4, m_Player.lock()->m_Transform.translation - m_Transform.translation);
				bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ 50 * (isFaceRight ? -1 : 1),0 } + glm::vec2{ bulletDirection[secondIndex].x * -150, bulletDirection[secondIndex].y * -150 };
				bullet->m_collider->size = { 10,10 };
				bullet->m_Transform.scale = { 2.5f,2.5f };
				bullet->m_Transform.rotation = (angle - secondIndex * 30) * (M_PI / 180.0);
				m_IRangedAttack->m_BulletBox->AddBullet(bullet);
				if (secondIndex == bulletDirection.size() - 1) secondIndex = -1;
			}
			m_IRangedAttack->isFire = true;
		}
		if (currentTime - m_FirstAttackTIme >= m_attackTime) {
			currentAttackState = StopShoot;
		}
	}
}
std::vector<std::shared_ptr<Object>> SCP743::summon() {
	isSummon = false;
	currentAttackState = StopShoot;
	std::vector<std::shared_ptr<Object>> objs;
	for (int i = 0; i < 2; ++i) {
		std::shared_ptr<Enemy> mob;
		if (IsSummonA) mob = std::make_shared<SCP743A>();
		else mob = std::make_shared<SCP743B>(); 
		this->AddChild(mob);
		mob->Start();
		mob->isCameraOn = true;
		mob->SetPlayer(m_Player);
		mob->SetZIndex(GetZIndex() - 0.01f);
		auto playerDirection = normalize(m_Player.lock()->m_Transform.translation - m_Transform.translation);

		mob->m_collider->position = m_Transform.translation + glm::vec2{ (i) ? playerDirection.x * 120 : playerDirection.x * -120 ,  (i) ? playerDirection.y * 120 : playerDirection.y * -120 + m_collider->offset.y/2 };
		mob->m_Transform.translation = m_Transform.translation + glm::vec2{ (i) ? playerDirection.x * 120 : playerDirection.x * -120 ,  (i) ? playerDirection.y * 120 : playerDirection.y * -120 + m_collider->offset.y / 2 };
		mob->m_WorldCoord = m_WorldCoord + glm::vec2{ (i) ? playerDirection.x * 120 : playerDirection.x * -120 ,  (i) ? playerDirection.y * 120 : playerDirection.y * -120 + m_collider->offset.y / 2 };

		objs.push_back(mob);
	}
	IsSummonA = !IsSummonA;
	return objs;
}