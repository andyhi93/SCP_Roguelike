#include "Bullet.hpp"

#include "Util/Logger.hpp"
#include <iostream>
#include <SDL.h>
#include "Player.hpp"

Bullet::Bullet(glm::vec2 pos, int _damage, CollisionLayer _layer, float _speed, int _imageID, glm::vec2 _direction) :Actor(glm::vec2(2, 2)){
	layer = _layer;
	m_collider->isTrigger = true;
	m_collider->tag = "Bullet";
	m_Transform.translation = pos;
	std::vector<std::string> bullet_images = { "../../../Resources/purple_ammo.png","../../../Resources/red_ammo.png","../../../Resources/choco_ammo.png" };
	m_Transform.scale = { 1,1 };
	this->SetDrawable(std::make_shared<Util::Image>(bullet_images[_imageID]));
	speed = _speed;
	damage = _damage;
	direction = _direction;
	direction = { direction.x / sqrt(direction.x * direction.x + direction.y * direction.y),direction.y / sqrt(direction.x * direction.x + direction.y * direction.y) };
}
void Bullet::Start() {
	m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));//for trigger func
	m_collider->parentActor = shared_from_this();
}
void Bullet::Update(){
	OutOfBounds();
}
void Bullet::FixedUpdate() {
	Move();
}
void Bullet::OutOfBounds() {
	float x = m_Transform.translation.x;
	float y = m_Transform.translation.y;

	if (x < -1920 || x > 1920 || y < -1080 || y > 1080) {
		islive = false;
	}
}
void Bullet::Move() {
	MoveX(direction.x * speed);
	MoveY(direction.y * speed);
}

void Bullet::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Wall" || other->tag == "Door0" || other->tag == "Door1" || other->tag == "Door2" || other->tag == "Door3") {
		islive = false;
		return;
	}
	std::shared_ptr<Object> collidedActor = other->parentActor;
	if (layer == CollisionLayer::Player && other->tag == "Enemy") {
		std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(collidedActor);
		if (enemy && !enemy->isDead) {
			enemy->SetHealth(enemy->GetHealth() - damage);
			std::cout << "Bullet hit Enemy" << std::endl;
			islive = false;
			return;
		}
	}
	else if (layer == CollisionLayer::Enemy && other->tag == "Player") {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collidedActor);
		if (!player->GetIsInvincible()) {
			player->Damage(1);
			std::cout << "Bullet hit Player" << std::endl;
			islive = false;
		}
	}
}
bool Bullet::GetIslive() { return islive; }
