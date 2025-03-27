#include "Enemies/SCP1048_C.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
SCP1048_C::SCP1048_C() : Enemy(glm::vec2{ 47,76 }) {
	isDropCoin = true;

	m_collider->SetTriggerCallback(std::make_shared<Trigger>());

	health = 1;
	speed = 1.0f;
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/SCP1048/SCP1048_C_walk1.png", RESOURCE_DIR "/SCP1048/SCP1048_C_walk2.png", }, true, 50, true, 50);
	m_AnimationAttack = m_AnimationWalk;
	m_AnimationDie = std::make_shared<Util::Animation>(
		std::vector<std::string>{RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png", }, true, 50, true, 50);
	SetDrawable(m_AnimationWalk);
	m_AnimationDie->SetLooping(false);
	m_AnimationWalk->Play();
	m_Transform.scale = { 4,4 };
}
void SCP1048_C::SetActive(bool isActive) {
	m_collider->isActive = isActive;
	if (m_meleeTrigger) m_meleeTrigger->m_collider->isActive = isActive;
	if (!isActive && m_IRangedAttack) m_IRangedAttack->m_BulletBox->ChangeRoom();
}
void SCP1048_C::SetPlayer(std::weak_ptr<Player> _player) {
	m_Player = _player;
}
void SCP1048_C::OnCollisionEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player") {
		m_Player.lock()->Damage(damage);
	}
}
void SCP1048_C::Behavior() {
	Shootable();
	m_meleeTrigger->FlipTrigger();

	glm::vec2 direction = normalize( m_Player.lock()->m_Transform.translation - m_Transform.translation);
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
}
void SCP1048_C::FixedUpdate() {
	m_IRangedAttack->m_BulletBox->FixedUpdate();
}
void SCP1048_C::Start() {
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
void SCP1048_C::Update() {
	m_IRangedAttack->m_BulletBox->Update();
	if (health <= 0 && !isDead) {
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
void SCP1048_C::Shootable() {
	m_IRangedAttack->currentTime = SDL_GetTicks() / 1000.0f;
	if (m_IRangedAttack->currentTime - m_IRangedAttack->m_LastShootTime >= m_IRangedAttack->shootSpeed) {
		m_IRangedAttack->m_LastShootTime = m_IRangedAttack->currentTime;
		m_IRangedAttack->isFire = false;
	}
	if (m_AnimationAttack->GetCurrentFrameIndex() == 1 && !m_IRangedAttack->isFire) {
		std::vector<glm::vec2> bulletDirection;
		if(shootIndex){
			bulletDirection = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{-1,-1},{-1,1},{1,-1} };
			shootIndex = 0;
		}
		else{
			bulletDirection = { {1,0.5},{1,-0.5},{-1,0.5},{-1,-0.5},{0.5,1},{0.5,-1},{-0.5,1},{-0.5,-1}, };
			shootIndex = 1;
		}
		for (int i = 0; i < 8; i++) {
			auto bullet = std::make_shared<Bullet>(m_Transform.translation, damage, CollisionLayer::Enemy, 10.0f, 1, bulletDirection[i]);
			m_IRangedAttack->m_BulletBox->AddBullet(bullet);
		}
		m_IRangedAttack->isFire = true;
		SetDrawable(m_AnimationWalk);
	}
}