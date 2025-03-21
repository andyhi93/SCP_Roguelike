//
// Created by SengQ1011
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Object.hpp"
#include "Util/Transform.hpp"
#include "pch.hpp"

class Camera{
public:
	Camera(std::vector<std::weak_ptr<Object>> pivotChildren = {});
	~Camera() = default;

	void CameraFollowWith(glm::vec2 target);

	[[nodiscard]] Util::Transform GetCameraWorldCoord()  { return m_CameraWorldCoord; }

	void AddRelativePivotChild( std::weak_ptr<Object> child);
	void RemoveRelativePivotChild( std::weak_ptr<Object> child);

	void Update();

protected:
	Util::Transform m_CameraWorldCoord;
private:
	std::vector<std::weak_ptr<Object>> m_RelativePivotChildren;
    glm::vec2 GetCursorPosition() {
        int win_w, win_h;
        SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &win_w, &win_h);

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        // 轉換成以視窗中心為 (0,0) 的座標
        float cursor_x = static_cast<float>(mouse_x) - static_cast<float>(win_w) / 2;
        float cursor_y = static_cast<float>(mouse_y) - static_cast<float>(win_h) / 2;

        // 翻轉 Y 軸 (因為螢幕座標系統通常是左上角為 (0,0)，而遊戲座標系統是中心為 (0,0))
        cursor_y = -cursor_y;

        return glm::vec2(cursor_x, cursor_y);
    }
};

#endif