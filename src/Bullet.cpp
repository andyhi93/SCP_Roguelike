#include "Bullet.hpp"

#include "Util/Logger.hpp"
#include <iostream>
#include <SDL.h>
#include "Player.hpp"

Bullet::Bullet(glm::vec2 pos, float _damage, CollisionLayer _layer, float _speed, int _imageID, glm::vec2 _direction) :Actor(glm::vec2(2, 2)){
	layer = _layer;
	m_collider->isTrigger = true;
	m_collider->tag = "Bullet";
	m_Transform.translation = pos;
	m_collider->position = pos;
	std::vector<std::string> bullet_images = { RESOURCE_DIR "/purple_ammo.png", RESOURCE_DIR "/red_ammo.png",RESOURCE_DIR "/choco_ammo.png",
		RESOURCE_DIR "/SCP049/knife.png",RESOURCE_DIR "/BigChoco_ammo.png" };
	m_Transform.scale = { 1,1 };
	this->SetDrawable(std::make_shared<Util::Image>(bullet_images[_imageID]));
	speed = _speed;
	damage = _damage;
	direction = _direction;
	direction = { direction.x / sqrt(direction.x * direction.x + direction.y * direction.y),direction.y / sqrt(direction.x * direction.x + direction.y * direction.y) };
	direction = { direction.x * speed ,direction.y * speed };
	//std::cout << "speed: " << direction.x << " ," << direction.y << "\n";
}
void Bullet::Start() {
	m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));//for trigger func
	m_collider->parentActor = shared_from_this();
}
void Bullet::Update(){
	OutOfBounds();
	Move();
}
void Bullet::FixedUpdate() {
}
void Bullet::OutOfBounds() {
	float x = m_Transform.translation.x;
	float y = m_Transform.translation.y;

	if (x < -1920 || x > 1920 || y < -1080 || y > 1080) {
		std::cout << "out of screen\n";
		islive = false;
	}
}
void Bullet::Move() {
	MoveX(direction.x);
	MoveY(direction.y);
}

void Bullet::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Wall" || other->tag == "Door0" || other->tag == "Door1" || other->tag == "Door2" || other->tag == "Door3") {
		islive = false;
		//std::cout << "bullet hit wall\n";
		return;
	}
	std::shared_ptr<Object> collidedActor = other->parentActor.lock();
	if (layer == CollisionLayer::Player && other->tag == "Enemy") {
		std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(collidedActor);
		if (enemy && !enemy->isDead) {
			enemy->Damage(damage);
			std::cout << "Bullet hit Enemy" << std::endl;
			islive = false;
			return;
		}
	}
	else if (layer == CollisionLayer::Enemy && other->tag == "Player") {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collidedActor);
		if (!player->GetIsInvincible() && !player->isDashing) {
			player->Damage(1);
			std::cout << "Bullet hit Player" << std::endl;
			islive = false;
		}
	}
}
