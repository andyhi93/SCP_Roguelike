#include "Hand.hpp"

#include "Util/Input.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"

Hand::Hand(std::shared_ptr<Player> player)
    : m_Player(player), m_Animation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
    "../../../Resources/Player/hand.png",
},
true, 50, true, 1000)) {
    m_Transform.translation = { 0, 0 };
    SetDrawable(m_Animation);
    m_Animation->Play();
    m_Transform.scale = { 5,5 };
}
void Hand::Update(){
    //hand follow
    glm::vec2 offset = { -12, 20 };
    m_Transform.translation = m_Player->GetTransform().translation+ offset;

    //aiming
    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    glm::vec2 direction = mousePos - m_Transform.translation;
    m_Transform.rotation = atan2(direction.y, direction.x) +90.0f;
}
