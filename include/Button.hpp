#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "pch.hpp"
#include "Util/Input.hpp"

#include "core/Object.hpp"
#include <Util/Image.hpp>

class Button : public Object {
public:
    Button(glm::vec2 pos, glm::vec2 size, float scaleSize, std::vector<std::string> path) {
        m_Transform.translation = pos;
        touchSize = size;
        m_Transform.scale = { scaleSize,scaleSize };
        imagePath = path;
        SetDrawable(std::make_shared<Util::Image>(imagePath[0]));
    }
    std::vector<std::string> imagePath;
    glm::vec2 touchSize;
    bool isActive = false;
    bool isTouch = false;
    void Update() {
        if (isActive) {
            if (GetCursorPosition().x< m_Transform.translation.x + touchSize.x / 2 && GetCursorPosition().x > m_Transform.translation.x - touchSize.x / 2 &&
                GetCursorPosition().y< m_Transform.translation.y + touchSize.y / 2 && GetCursorPosition().y > m_Transform.translation.y - touchSize.y / 2) {
                if (!isTouch) {
                    SetDrawable(std::make_shared<Util::Image>(imagePath[1]));
                    isTouch = true;
                }
            }
            else {
                if (isTouch) {
                    SetDrawable(std::make_shared<Util::Image>(imagePath[0]));
                    isTouch = false;
                }
            }
        }
    }
    bool isClick(){
        if (isActive) {
            if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && GetCursorPosition().x< m_Transform.translation.x + touchSize.x / 2 && GetCursorPosition().x > m_Transform.translation.x - touchSize.x / 2 &&
                GetCursorPosition().y< m_Transform.translation.y + touchSize.y / 2 && GetCursorPosition().y > m_Transform.translation.y - touchSize.y / 2) {
                return true;
            }
        }
        return false;
    }
private:
};

#endif
