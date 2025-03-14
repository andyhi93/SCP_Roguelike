#include "Chest.hpp"
#include <random>
#include "Util/Image.hpp"

Chest::Chest(glm::vec2 pos, glm::vec2 size) : Solid(pos,size){
	m_collider->tag = "Chest";
	m_Transform.scale = { 1.5f, 1.5f };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(Item::SCP1609, Item::SCP2295);
	treasure = std::make_shared<Item>(pos + glm::vec2{ 0,-150 }, static_cast<Item::ItemType>(dis(gen)));
	treasure->Start();
	treasure->m_collider->isActive = false;
	this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/Box1.png"));

	openArea = std::make_shared<Actor>(glm::vec2{ 200,200 });//levelmanager need open isActive
	openArea->m_collider->tag = "ChestTrigger";
	openArea->m_collider->isTrigger = true;
	openArea->m_collider->isActive = false;
	
}
void Chest::Start() {
	openArea->m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Chest::SetActive(bool IsActive) {
	openArea->m_collider->isActive = IsActive;
	m_collider->isActive = IsActive;
}
void Chest::Open(){
	isOpen = true;
	openArea->m_collider->isActive = false;
	this->AddChild(treasure);
	treasure->m_collider->isActive = true;
	this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/Box2.png"));
}

void Chest::Update(){
	//std::cout << "Chest isActive: " << m_collider->isActive << "\n";
	if (treasure && treasure->isPick) {
		this->RemoveChild(treasure);
		treasure = nullptr;
	}
}