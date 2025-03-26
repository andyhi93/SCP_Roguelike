#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "pch.hpp"
#include "Util/Input.hpp"

#include "core/Object.hpp"
#include <Util/Image.hpp>

class Slider : public Object {
public:
    Slider(glm::vec2 pos, glm::vec2 size, float scaleSize, std::vector<std::string> path) {
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
    int eventIndex;
    void Update() {
        if (isActive) {
            if (GetCursorPosition().x< m_Transform.translation.x + touchSize.x / 2 && GetCursorPosition().x > m_Transform.translation.x - touchSize.x / 2 &&
                GetCursorPosition().y< m_Transform.translation.y + touchSize.y / 2 && GetCursorPosition().y > m_Transform.translation.y - touchSize.y / 2) {
                if (!isTouch) {
                    isTouch = true;
                }
            }
            else {
                if (isTouch) {
                    isTouch = false;
                }
            }
        }
    }
    bool isClicking() {
        if (isActive) {
            if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && GetCursorPosition().x< m_Transform.translation.x + touchSize.x / 2 && GetCursorPosition().x > m_Transform.translation.x - touchSize.x / 2 &&
                GetCursorPosition().y< m_Transform.translation.y + touchSize.y / 2 && GetCursorPosition().y > m_Transform.translation.y - touchSize.y / 2) {
                return true;
            }
        }
        return false;
    }
private:
};

#endif
