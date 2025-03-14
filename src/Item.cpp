#include "Item.hpp"
#include <Util/Image.hpp>
#include <random>


int Item::lastItemIndex = SCP2295;
Item::Item(glm::vec2 pos, ItemType type) :Actor(glm::vec2{ 80,80 }) {
	m_collider->isTrigger = true;
	m_Transform.translation = pos;
	m_collider->position = m_Transform.translation + m_collider->offset;
	m_Transform.scale = { 3,3 };
	SetItemType(type);
	std::cout << "Item: " << type<<std::endl;
}
std::shared_ptr<Item> Item::SetSell() {
	isUnlocked = false;
	m_priceText = std::make_unique<UIText>(RESOURCE_DIR "/UI/PixelText.ttf", 50);
	m_priceText->SetZIndex(this->GetZIndex()+0.2f);
	m_priceText->Start();
	m_priceText->m_Transform.translation = m_Transform.translation + glm::vec2{0,-70};
	m_priceText->m_Transform.scale = { 0.7,0.7 };
	m_priceText->m_Text->SetText(fmt::format("{}", price));
	this->AddChild(m_priceText);

	backgroundImage= std::make_shared<Object>();
	backgroundImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/shopUI.png"));
	backgroundImage->SetZIndex(this->GetZIndex() + 0.1f);
	backgroundImage->m_Transform.scale = { 2,2 };
	backgroundImage->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,-70 };
	this->AddChild(backgroundImage);

	return std::dynamic_pointer_cast<Item>(shared_from_this());
}
void Item::SetItemType(ItemType type) {
	m_collider->tag = "Item";
	m_collider->isTrigger = true;
	ItemIndex = type;
	if (type == ItemType::bloodCoin) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 2);
		m_Transform.scale = { 2,2 };
		price = 1;
		int cmd = dis(gen);
		if (cmd == 1) {
			price = 5;
			m_Transform.scale = { 3,3 };
		}
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/bloodCoin.png"));
	}
	if (type == ItemType::bloodPill) {
		healValue = 2;
		price = 3;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/bloodPill.png"));
	}
	if (type == ItemType::SCP023) {
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP023.png"));
	}
	if (type == ItemType::SCP682) {
		healthUp = 1.4f;
		damageUp = 1.4f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP682.png"));
	}
	if (type == ItemType::SCP610) {
		healthUp = 1.2f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP610.png"));
	}
	if (type == ItemType::SCP668) {
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP668.png"));
	}
	if (type == ItemType::SCP063) {
		damageUp = 1.3f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP063.png"));
	}
	if (type == ItemType::SCP1016) {
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP1016.png"));
	}
	if (type == ItemType::SCP1609) {
		moveSpeedUp = 1.2f;
		dashCooldown = 0.9f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP1609.png"));
	}
	if (type == ItemType::SCP2295) {
		healthUp = 1.2f;
		healValue = 10;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP2295.png"));
	}
	if (type == ItemType::SCP307) {
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP307.png"));
	}
	if (type == ItemType::SCP553) {
		damageUp = 1.1f;
		moveSpeedUp = 1.3f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP553.png"));
	}
}
std::vector<float> Item::pickUp() {
	std::vector<float> packageValue = {
		attackSpeedUp,
		moveSpeedUp,
		damageUp,
		healthUp,
		dashCooldown,
		healValue,
		price,
	};
	isPick = true;
	return packageValue;
}
void Item::Update() {

}
void Item::Start() {
	m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
	m_collider->position = m_Transform.translation;
}