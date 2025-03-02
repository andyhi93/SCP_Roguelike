#include "Hand.hpp"

#include "Util/Input.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"

Hand::Hand()
    :  m_Animation(std::make_shared<Util::Animation>(
        std::vector<std::string>{
    "../../../Resources/Player/hand.png",
},
true, 50, true, 1000)) {
    m_AnimationDash = std::make_shared<Util::Animation>(std::vector<std::string>{"../../../Resources/Player/hand_dash.png"}, true, 1000, true, 1000);
    m_Transform.translation = { 0, 0 };
    SetDrawable(m_Animation);
    m_Animation->Play();
    m_Transform.scale = { 5,5 };
}
void Hand::Update(){

    //aiming
}
