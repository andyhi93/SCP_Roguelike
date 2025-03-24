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
std::shared_ptr<Item> Item::SetPickUI() {
	this->SetVisible(false);
	if(m_priceText) m_priceText->SetVisible(false);
	if(backgroundImage) backgroundImage->SetVisible(false);

	float currentTime = SDL_GetTicks() / 1000.0f;
	endTime = currentTime + 5;

	m_nameDescription = std::make_unique<UIText>(RESOURCE_DIR "/UI/PixelText.ttf", 50);
	m_nameDescription->SetZIndex(this->GetZIndex() + 10.1f);
	m_nameDescription->Start();
	m_nameDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,105 };
	m_nameDescription->m_Transform.scale = { 0.7,0.7 };
	m_nameDescription->m_Text->SetText(fmt::format("{}", nameText));
	this->AddChild(m_nameDescription);

	m_effectDescription = std::make_unique<UIText>(RESOURCE_DIR "/UI/PixelText.ttf", 50);
	m_effectDescription->SetZIndex(this->GetZIndex() + 10.1f);
	m_effectDescription->Start();
	m_effectDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,-207 };
	m_effectDescription->m_Transform.scale = { 0.7,0.7 };
	m_effectDescription->m_Text->SetText(fmt::format("{}", effectText));
	this->AddChild(m_effectDescription);

	descriptionBackgroundImage = std::make_shared<Object>();
	descriptionBackgroundImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/itemBackground.png"));
	descriptionBackgroundImage->SetZIndex(this->GetZIndex() + 10);
	descriptionBackgroundImage->m_Transform.scale = { 3,3 };
	descriptionBackgroundImage->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,-50 };
	this->AddChild(descriptionBackgroundImage);

	return std::dynamic_pointer_cast<Item>(shared_from_this());
}
void Item::SetItemType(ItemType type) {
	m_collider->tag = "Item";
	m_collider->isTrigger = true;
	ItemIndex = type;
	if (type == ItemType::bloodCoin) {
		hasDescripting = false;
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
		hasDescripting = false;
		healValue = 2;
		price = 3;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/bloodPill.png"));
	}
	if (type == ItemType::SCP023) {
		nameText = "EYE OF SCP-023\nBLACK SHUCK";
		effectText = "DAMAGE UP\nATTACK SPEED UP";
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP023.png"));
	}
	if (type == ItemType::SCP682) {
		nameText = "TOOTH OF SCP-682\nHARD-TO-DESTROY\nREPTILE";
		effectText = "DAMAGE UP\nHEALTH UP";
		healthUp = 1.4f;
		damageUp = 1.4f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP682.png"));
	}
	if (type == ItemType::SCP610) {
		nameText = "SCP-610\nTHE FLESH THAT\nHATES";
		effectText = "DAMAGE UP\nHEALTH UP";
		healthUp = 1.2f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP610.png"));
	}
	if (type == ItemType::SCP668) {
		nameText = "SCP-668\n13- CHEF'S KNIFE";
		effectText = "DAMAGE UP\nATTACK SPEED UP";
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP668.png"));
	}
	if (type == ItemType::SCP063) {
		nameText = "SCP-063\nTHE WORLD'S\nBEST TOTHBRUSH";
		effectText = "DAMAGE UP";
		damageUp = 1.3f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP063.png"));
	}
	if (type == ItemType::SCP1016) {
		nameText = "SCP-1016\nTHE BLOODY KEY";
		effectText = "DAMAGE UP\nATTACK SPEED UP";
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 10;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP1016.png"));
	}
	if (type == ItemType::SCP1609) {
		nameText = "SCP-1609\nTHE REMAINS OF\nA CHAIR";
		effectText = "DASH COOLDOWN DOWN\nMOVE SPEED UP";
		moveSpeedUp = 1.2f;
		dashCooldown = 0.9f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP1609.png"));
	}
	if (type == ItemType::SCP2295) {
		nameText = "SCP-2295\nTHE BEAR WITH  A\nHEART OF PATCHWORK";
		effectText = "HEALTH UP\nHEAL 10 HP";
		healthUp = 1.2f;
		healValue = 10;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP2295.png"));
	}
	if (type == ItemType::SCP307) {
		nameText = "SCP-307\nCARNIVOROUS IVY";
		effectText = "DAMAGE UP\nATTACK SPEED UP";
		attackSpeedUp = 0.8f;
		damageUp = 1.2f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP307.png"));
	}
	if (type == ItemType::SCP553) {
		nameText = "WINGS OF SCP-553\nCRYSTALLINE\nBUTTERFLIES";
		effectText = "DAMAGE UP\nMOVE SPEED UP";
		damageUp = 1.1f;
		moveSpeedUp = 1.3f;
		price = 15;
		this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/SCP553.png"));
	}
}
std::vector<float> Item::pickUp() {
	m_UIeDescription->SetVisible(false);
	UIeBackgroundImage->SetVisible(false);
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
	if(hasDescripting) SetPickUI();
	return packageValue;
}
void Item::Update() {
	float currentTime = SDL_GetTicks() / 1000.0f;
	if (endTime != 0 && currentTime > endTime) {
		isDescripting = false;
	}
	if (isCameraOn) {
		if(m_nameDescription) m_nameDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,105 };
		if(m_effectDescription)m_effectDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,-207 };
		if(descriptionBackgroundImage)descriptionBackgroundImage->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,-50 };
		m_UIeDescription->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,113 };
		UIeBackgroundImage->m_Transform.translation = m_Transform.translation + glm::vec2{ 0,113 };
	}
}
void Item::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player" && !isPick && hasDescripting) {
		m_UIeDescription->SetVisible(true);
		UIeBackgroundImage->SetVisible(true);
	}
}
void Item::OnTriggerExit(std::shared_ptr<BoxCollider> other) {
	if (other->tag == "Player" && !isPick && hasDescripting) {
		m_UIeDescription->SetVisible(false);
		UIeBackgroundImage->SetVisible(false);
	}
}
void Item::Start() {
	m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));

	m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
	m_collider->position = m_Transform.translation;

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