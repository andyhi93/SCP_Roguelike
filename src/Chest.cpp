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
void Chest::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player" && !isOpen) {
		m_UIeDescription->SetVisible(true);
		UIeBackgroundImage->SetVisible(true);
	}
}
void Chest::OnTriggerExit(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player" && !isOpen) {
		m_UIeDescription->SetVisible(false);
		UIeBackgroundImage->SetVisible(false);
	}
}
void Chest::Start() {
	openArea->m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());

	openArea->m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));

	m_UIeDescription = std::make_unique<UIText>(RESOURCE_DIR "/UI/PixelText.ttf", 50);
	m_UIeDescription->SetVisible(false);
	m_UIeDescription->SetZIndex(this->GetZIndex() + 10.1f);
	m_UIeDescription->Start();
	m_UIeDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,113 };
	m_UIeDescription->m_Transform.scale = { 0.7,0.7 };
	m_UIeDescription->m_Text->SetText(fmt::format("{}", UIeText));
	this->AddChild(m_UIeDescription);

	UIeBackgroundImage = std::make_shared<Object>();
	UIeBackgroundImage->SetVisible(false);
	UIeBackgroundImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/background.png"));
	UIeBackgroundImage->SetZIndex(this->GetZIndex() + 10);
	UIeBackgroundImage->m_Transform.scale = { 3,3 };
	UIeBackgroundImage->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,113 };
	this->AddChild(UIeBackgroundImage);
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

	m_UIeDescription->SetVisible(false);
	UIeBackgroundImage->SetVisible(false);
}

void Chest::Update(){
	if (isCameraOn) {
		m_collider->position = m_Transform.translation + m_collider->offset;
		//std::cout << "pos: " << m_Transform.translation.x << ", " << m_Transform.translation.y << "\nColPos: " << m_collider->position.x << ", " << m_collider->position.y << "\n";
		m_UIeDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,113 };
		UIeBackgroundImage->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,113 };
		openArea->m_collider->position = m_Transform.translation;
		if (treasure) {
			treasure->isCameraOn = true;
			treasure->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,-150 };
			treasure->m_collider->position = m_Transform.translation + glm::vec2{ 0,-150 };
			treasure->Update();
		}
	}
	if (treasure && ((!treasure->hasDescripting && treasure->isPick) || (treasure->hasDescripting && !treasure->isDescripting))) {
		this->RemoveChild(treasure);
		treasure = nullptr;
	}
}