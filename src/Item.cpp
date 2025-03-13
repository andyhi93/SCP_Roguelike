#include "Item.hpp"

Item::Item(glm::vec2 pos, ItemType type) :Actor(glm::vec2{ 50,50 }) {
	m_collider->isTrigger = true;
	m_Transform.translation = pos;
	m_collider->position = m_Transform.translation + m_collider->offset;
	SetItemType(type);
}
void Item::SetItemType(ItemType type) {
	attackSpeed = 1;
	movekSpeed = 1;
	damageUp = 1;
	healthUp = 1;
	dashCooldown = 1;
	healValue = 0;
	if (type == ItemType::redPill) {
		healValue = 2;
	}
	if (type == ItemType::SCP023) {
		attackSpeed = 1.2f;
		damageUp = 1.2f;
	}
	if (type == ItemType::SCP063) {
		damageUp = 1.3f;
	}
	if (type == ItemType::SCP1016) {
		attackSpeed = 1.2f;
		damageUp = 1.2f;
	}
	if (type == ItemType::SCP1609) {
		movekSpeed = 1.2f;
		dashCooldown = 0.9f;
	}
	if (type == ItemType::SCP2295) {
		healthUp = 1.2f;
		healValue = 10;
	}
	if (type == ItemType::SCP297) {
		damageUp = 1.4f;
	}
	if (type == ItemType::SCP307) {
		attackSpeed = 1.2f;
		damageUp = 1.2f;
	}
	if (type == ItemType::SCP553) {
		damageUp = 1.2f;
		movekSpeed = 1.3f;
	}
}
void Item::pickUp() {
	isPick = true;
}
void Item::Update() {

}
void Item::Start() {
	m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
	m_collider->position = m_Transform.translation;
	ColliderManager::GetInstance().RegisterCollider(m_collider);
}