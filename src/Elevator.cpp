#include "Elevator.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include "random"
#include <Enemy.hpp>

Elevator::Elevator(glm::vec2 pos, glm::vec2 size) : Solid(pos,size) {
	SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Room/elevator.png"));
	m_Transform.translation = pos;
	m_Transform.scale = { 4,4 };
	m_collider->tag = "Elevator";
	m_collider->isSolid = true;
	m_collider->isTrigger = true;
	m_collider->isActive = false;
	m_collider->position = m_Transform.translation + m_collider->offset;
}
void Elevator::Update() {
	if (isCameraOn) {
		m_collider->position = m_Transform.translation + m_collider->offset;
	}
}
void Elevator::Start() {
	//m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Elevator::SetActive(bool active) {
	SetVisible(active);
	if(!isStartRoom) m_collider->isActive = active;
}