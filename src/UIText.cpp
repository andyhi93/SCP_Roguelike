#include "UIText.hpp"

#include "Util/Color.hpp"
#include "Util/Time.hpp"

void UIText::Start() {
    m_Text = std::make_unique<Util::Text>(m_Font, m_Size, "0",
        Util::Color::FromRGB(255, 255, 255));
    SetDrawable(m_Text);
}

void UIText::Update() {
}
