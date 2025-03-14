#include "Chest.hpp"
#include <random>
#include "Util/Image.hpp"

Chest::Chest(glm::vec2 pos, glm::vec2 size) : Solid(pos,size){
	m_collider->tag = "Chest";
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(Item::SCP1609, Item::SCP2295);
	treasure = std::make_shared<Item>(pos + glm::vec2{ 0,-50 }, static_cast<Item::ItemType>(dis(gen)));
	this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/Box.png"));

	openArea = std::make_shared<Actor>(glm::vec2{ 150,100 });//levelmanager need open isActive
	openArea->m_collider->isTrigger = true;
	openArea->m_collider->isActive = false;
	
}
void Chest::Open(){
	isOpen = true;
	this->AddChild(treasure);
	treasure->m_collider->isActive = true;
}

void Chest::Update(){
	std::cout << "Chest isActive: " << m_collider->isActive << "\n";
}