#include "Table.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include "random"

Table::Table(glm::vec2 pos, glm::vec2 size) : Solid(pos, size) {
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> dis(0,1);
    tableIndex = dis(gen) * 2;
    tableImages = { "../../../Resources/table.png" ,"../../../Resources/table_broken.png",
    "../../../Resources/tablePS5.png" ,"../../../Resources/tablePS5_broken.png", };
    SetDrawable(std::make_shared<Util::Image>(tableImages[tableIndex]));
    m_Transform.translation = pos;
    m_Transform.scale = { 7, 7 };
    m_collider->tag = "Table";
    m_collider->isSolid = true;
}
void Table::Start() {
    m_collider->parentActor= std::dynamic_pointer_cast<Object>(shared_from_this());
}
void Table::SetTrigger() {
    m_collider->isTrigger = !m_collider->isTrigger;
}
void Table::BreakTable() {
    if (!isBroken) {
        isBroken = true;
        m_collider->isActive = false;
        SetDrawable(std::make_shared<Util::Image>(tableImages[++tableIndex]));
    }
}
void Table::Update() {
}
