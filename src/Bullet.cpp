#include "Bullet.hpp"

#include "Util/Logger.hpp"
#include <iostream>
#include <SDL.h>
#include "LevelManager.hpp"
#include "Player.hpp"

Bullet::Bullet(int _damage, GameObject::CollisionLayer _layer, float _speed, int _imageID, glm::vec2 _direction){
	m_Collider = std::make_shared<Collider>(m_Transform.translation,glm::vec2(2,2));
	std::vector<std::string> bullet_images = { "../../../Resources/purple_ammo.png","../../../Resources/red_ammo.png" };
	m_Transform.scale = { 1,1 };
	this->SetDrawable(std::make_shared<Util::Image>(bullet_images[_imageID]));
	speed = _speed;
	damage = _damage;
	direction = _direction;
	layer = _layer;

	/*std::cout << "Bullet created with damage: " << damage
		<< ", team: " << team
		<< ", speed: " << speed
		<< ", direction: (" << direction.x << ", " << direction.y << ")\n";*/
}
void Bullet::getLevelManager(std::shared_ptr<LevelManager> _LevelManager) {
	m_LevelManager = _LevelManager;
}
void Bullet::getPlayer(std::shared_ptr<Player> _Player) {
	m_Player = _Player;
}
void Bullet::Update(){
	Move();
	Hit();
	IsOutOfBounds();
}
bool Bullet::IsOutOfBounds() {
	float x = m_Transform.translation.x;
	float y = m_Transform.translation.y;

	if (x < -1920 || x > 1920 || y < -1080 || y > 1080) {
		return true;
	}
	return false;
}
void Bullet::Move(){
	this->m_Transform.translation += direction*speed;
}
void Bullet::Hit(){
	if (IsOutOfBounds()) {
		islive = false;
		std::cout << "Bullet out of screen" << std::endl;
	}
	m_Collider->position = this->m_Transform.translation + glm::vec2(-1, -1);
	const std::vector<Collider>& walls = Collider::GetWalls();
	for (const auto& wall : walls){
		if (m_Collider->CheckCollision(*m_Collider, wall)) {
			islive = false;
			return;
		}
	}
	const std::vector<Collider>& doors = Collider::GetDoors();
	for (const auto& door : doors) {
		if (m_Collider->CheckCollision(*m_Collider, door)) {
			islive = false;
			return;
		}
	}
	//wtf
	if (layer == CollisionLayer::Player) {
		for (auto& enemyObj : m_LevelManager->currentEnemies) {
			if (m_Collider->CheckCollision(*m_Collider, *enemyObj->m_Collider)) {
				enemyObj->SetHealth(enemyObj->GetHealth() - damage);
			}
		}
	}
	if (layer == CollisionLayer::Enemy && m_Collider->CheckCollision(*m_Collider, *m_Player->m_Collider)) {
		m_Player->health -= damage;
	}
}
bool Bullet::GetIslive() { return islive; }
