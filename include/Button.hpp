#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "pch.hpp"
#include "Util/Input.hpp"

#include "core/Object.hpp"
#include <Util/Image.hpp>

class Button : public Object {
public:
    Button(glm::vec2 pos, glm::vec2 size, float scaleSize, std::vector<std::string> path, int eventIndex) {
        m_Transform.translation = pos;
        touchSize = size;
        m_Transform.scale = { scaleSize,scaleSize };
        imagePath = path;
        this->eventIndex = eventIndex;
        SetDrawable(std::make_shared<Util::Image>(imagePath[0]));
    }
    std::vector<std::string> imagePath;
    glm::vec2 touchSize;
    bool isActive = false;
    bool isTouch = false;
    int eventIndex;
    void Update() {
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
    bool isClick(){
        if (Util::Input::IsKeyUp(Util::Keycode::MOUSE_LB) && GetCursorPosition().x< m_Transform.translation.x + touchSize.x / 2 && GetCursorPosition().x > m_Transform.translation.x - touchSize.x / 2 &&
            GetCursorPosition().y< m_Transform.translation.y + touchSize.y / 2 && GetCursorPosition().y > m_Transform.translation.y - touchSize.y / 2) {
            return true;
        }
        return false;
    }
    int GetEventIndex() { return eventIndex; }
private:
    glm::vec2 GetCursorPosition() {
        int win_w, win_h;
        SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &win_w, &win_h);

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // �ഫ���H�������߬� (0,0) ���y��
        float cursor_x = static_cast<float>(mouse_x) - static_cast<float>(win_w) / 2;
        float cursor_y = static_cast<float>(mouse_y) - static_cast<float>(win_h) / 2;

        // ½�� Y �b (�]���ù��y�Шt�γq�`�O���W���� (0,0)�A�ӹC���y�Шt�άO���߬� (0,0))
        cursor_y = -cursor_y;

        return glm::vec2(cursor_x, cursor_y);
    }
};

#endif
