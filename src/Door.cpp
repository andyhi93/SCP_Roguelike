#include "Door.hpp"

#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <iostream>

Door::Door(glm::vec2 pos,glm::vec2 size):Solid(pos, size){

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
        m_collider->isTrigger = true;
        this->SetDrawable(std::make_shared<Util::Image>(doorImage[1]));
    }
    else {
        isOpen = false;
        m_collider->isTrigger = false;
        this->SetDrawable(std::make_shared<Util::Image>(doorImage[0]));
    }
}
void Door::Update() {
}
