#include "Trap.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include "random"

Trap::Trap(glm::vec2 pos, glm::vec2 size) : Solid(pos, size) {

	attackSpeed = 3;
    trapImages = { RESOURCE_DIR "/Trap1.png" ,RESOURCE_DIR "/Trap2.png" };
    SetDrawable(std::make_shared<Util::Image>(trapImages[0]));
    m_Transform.translation = pos;
    m_Transform.scale = { 3, 3 };
    m_collider->tag = "Trap";
    m_collider->isSolid = true;
	m_collider->isTrigger = true;
	m_collider->isActive = false;
}
void Trap::Update(){
	if (isOpen) {
		float currentTime = SDL_GetTicks() / 1000.0f;
		if (currentTime - m_LastAttackTime >= attackSpeed) {
			SetActive();
			m_LastAttackTime = currentTime;
		}
		if (m_collider->isActive && currentTime - m_LastAttackTime >= attackTime) {
			SetActive();
		}
	}
}
void Trap::Start(){
	float currentTime = SDL_GetTicks() / 1000.0f;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 4);
	m_LastAttackTime = currentTime+dis(gen);
	m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Trap::SetActive() {
	m_collider->isActive = !m_collider->isActive;
    SetDrawable(std::make_shared<Util::Image>(trapImages[m_collider->isActive]));
}