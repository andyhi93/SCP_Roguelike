#include "Door.hpp"

#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <iostream>

Door::Door(){
    m_Collider = std::make_shared<Collider>(m_Transform.translation, glm::vec2{ 45,140 });
    doorImage = { "../../../Resources/Door0.png", "../../../Resources/Door1.png" };
    m_Transform.scale = { 7,7 };
}
void Door::SetIsTop(bool _isTop) {
    doorImage = { "../../../Resources/TopDoor0.png","../../../Resources/TopDoor1.png" };
    isTop = _isTop;
}
void Door::DoorControl(bool cmd) {
    if (cmd) {
        isOpen = true;
        this->SetDrawable(std::make_shared<Util::Image>(doorImage[1]));
    }
    else {
        isOpen = false;
        this->SetDrawable(std::make_shared<Util::Image>(doorImage[0]));
    }
}
/*
void Door::Move(glm::vec2& velocity) {
    if (m_Collider->CheckCollision(*m_Collider, walls[0]) && velocity.y > 0) {
        velocity.y = 0;
    }
    if (m_Collider->CheckCollision(*m_Collider, walls[1]) && velocity.y < 0) {
        velocity.y = 0;
    }
    if (m_Collider->CheckCollision(*m_Collider, walls[2]) && velocity.x < 0) {
        velocity.x = 0;
    }
    if (m_Collider->CheckCollision(*m_Collider, walls[3]) && velocity.x > 0) {
        velocity.x = 0;
    m_Transform.translation += velocity;

}
    }*/
void Door::Update() {
}
