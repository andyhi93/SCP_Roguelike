#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Util/GameObject.hpp"
#include <Util/SFX.hpp>

class Object : public Util::GameObject {
public:
    glm::vec2 m_WorldCoord = { 0,0 };
    bool isCameraOn = false;
    virtual void Update(){}
    virtual void FixedUpdate(){}
    enum class CollisionLayer {
        Player = 1,
        Enemy = 2,
        Environment = 3,
    };
    CollisionLayer layer;

    glm::vec2 GetCursorPosition() {
        int win_w, win_h;
        SDL_GL_GetDrawableSize(SDL_GL_GetCurrentWindow(), &win_w, &win_h);

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // 轉換成以視窗中心為 (0,0) 的座標
        float cursor_x = static_cast<float>(mouse_x) - static_cast<float>(win_w) / 2;
        float cursor_y = static_cast<float>(mouse_y) - static_cast<float>(win_h) / 2;

        // 翻轉 Y 軸 (因為螢幕座標系統通常是左上角為 (0,0)，而遊戲座標系統是中心為 (0,0))
        cursor_y = -cursor_y;

        return glm::vec2(cursor_x, cursor_y);
    }
    std::shared_ptr<Util::SFX> m_SFX = std::make_shared<Util::SFX>("");
};

#endif
