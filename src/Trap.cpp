#include "Trap.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include "random"
#include <Enemy.hpp>

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
	m_collider->offset = glm::vec2{ 0,-20 };
	m_collider->position = m_Transform.translation + m_collider->offset;
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
void Trap::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
	std::cout << "Trap\n";
	if (other->tag == "Enemy" && isOpen) {
		auto enemy = std::dynamic_pointer_cast<Enemy>(other->parentActor.lock());
		if(enemy&& !enemy->canFly) enemy->Damage(1.0f);
	}
}
void Trap::Start(){
	float currentTime = SDL_GetTicks() / 1000.0f;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 4);
	m_LastAttackTime = currentTime+dis(gen);
	m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
	m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));//for trigger func
}
void Trap::SetActive() {
	m_collider->isActive = !m_collider->isActive;
    SetDrawable(std::make_shared<Util::Image>(trapImages[m_collider->isActive]));
}