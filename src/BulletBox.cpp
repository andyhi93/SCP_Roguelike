#include "BulletBox.hpp"
#include <iostream>

BulletBox::BulletBox(){}
void BulletBox::Update(){
	AutoRemove();
	for (auto& bullet : bullets) {
		bullet->Update();
	}
}
void BulletBox::FixedUpdate() {
	for (auto& bullet : bullets) {
		bullet->FixedUpdate();
	}
}
void BulletBox::AutoRemove() {
	size_t beforeSize = bullets.size();
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[this](const std::shared_ptr<Bullet>& bullet) {
			if (!bullet->GetIslive()) {
				ColliderManager::GetInstance().UnregisterCollider(bullet->m_collider);
				this->RemoveChild(bullet); 
				return true;
			}
			return false;
		}),
		bullets.end());
	size_t afterSize = bullets.size();
	if (beforeSize != afterSize) {
		std::cout << "Removed " << (beforeSize - afterSize) << " bullets still last " <<(afterSize)<<"bullets" << std::endl;
	}
}
void BulletBox::ChangeRoom() {
	for (auto& bullet : bullets) {
		bullet->SetIsLive(false);
	}
}
void BulletBox::RemoveAll() {
	bullets.clear();
}
void BulletBox::AddBullet(std::shared_ptr<Bullet> bullet) {
	bullet->Start();
	bullets.push_back(bullet);
	bullet->SetZIndex(this->GetZIndex()+0.5f);
	this->AddChild(bullet);
}
