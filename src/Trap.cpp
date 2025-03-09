#include "Trap.hpp"
#include "Util/Image.hpp"
#include <iostream>

Trap::Trap(glm::vec2 pos, glm::vec2 size) : Solid(pos, size) {
    trapImages = { "../../../Resources/Trap1.png" ,"../../../Resources/Trap2.png" };
    SetDrawable(std::make_shared<Util::Image>(trapImages[0]));
    m_Transform.translation = pos;
    m_Transform.scale = { 3, 3 };
    m_collider->tag = "Trap";
    m_collider->isSolid = true;
}
void Trap::Update(){}
void Trap::Start(){
	m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Trap::SetActive() {
	m_collider->isActive = !m_collider->isActive;
    SetDrawable(std::make_shared<Util::Image>(trapImages[m_collider->isActive]));
}