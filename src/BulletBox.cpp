#include "BulletBox.hpp"
#include <iostream>

BulletBox::BulletBox(){
	
}
void BulletBox::getLevelManager(std::shared_ptr<LevelManager> _LevelManager) {
	m_LevelManager = _LevelManager;
}
void BulletBox::getPlayer(std::shared_ptr<Player> _Player) {
	m_Player = _Player;
}
void BulletBox::Update(){
	AutoRemove();
	for (auto& bullet : bullets) {
		bullet->Update();
	}
}
void BulletBox::AutoRemove() {
	size_t beforeSize = bullets.size();
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
		[this](const std::shared_ptr<Bullet>& bullet) {
			if (!bullet->GetIslive()) {
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
void BulletBox::RemoveAll() {
	bullets.clear();
}
void BulletBox::AddBullet(std::shared_ptr<Bullet> bullet) {
	bullets.push_back(bullet);
	bullet->getLevelManager(m_LevelManager);
	bullet->getPlayer(m_Player);
	this->AddChild(bullet);
}
