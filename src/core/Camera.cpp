#include "Core/Camera.hpp"
#include <iostream>
#include <Util/Input.hpp>
#include <Util/Logger.hpp>
#include <Core/Actor.hpp>

Camera::Camera(std::vector<std::weak_ptr<Object>> pivotChildren) : m_RelativePivotChildren(pivotChildren){
	m_CameraWorldCoord.translation = { 0.0f,0.0f };
}
void Camera::CameraFollowWith(glm::vec2 target) { 
	m_CameraWorldCoord.translation = target; 
	//std::cout << "pos:" << (target- m_CameraWorldCoord.translation).x << " ," << (target - m_CameraWorldCoord.translation).y << "\n";
}

void Camera::AddRelativePivotChild(std::weak_ptr<Object> child) {
	if (child.expired()) {
		std::cerr << "Error: Attempted to add an expired weak_ptr to Camera!\n";
		return;
	}
	m_RelativePivotChildren.push_back(child);
}
void Camera::RemoveRelativePivotChild(std::weak_ptr<Object> child) {
	m_RelativePivotChildren.erase(
		std::remove_if(
			m_RelativePivotChildren.begin(), m_RelativePivotChildren.end(),
			[&](const std::weak_ptr<Object>& weakObj) {
				return !weakObj.owner_before(child) && !child.owner_before(weakObj);
			}
		),
		m_RelativePivotChildren.end()
	);
}
void Camera::Update() {
	for (auto& child : m_RelativePivotChildren){
		if (auto lockedChild = child.lock()) {  // 確保 weak_ptr 仍然有效
			lockedChild->m_Transform.translation = lockedChild->m_WorldCoord - m_CameraWorldCoord.translation;
			auto actor = std::dynamic_pointer_cast<Actor>(lockedChild);
			if (actor) actor->m_collider->position = actor->m_Transform.translation + actor->m_collider->offset;
			//if(lockedChild->layer== Object::CollisionLayer::Player) std::cout << "pos:" << (lockedChild->m_WorldCoord - m_CameraWorldCoord.translation).x << " ," << (lockedChild->m_WorldCoord - m_CameraWorldCoord.translation).y << "\n";
		}
	}
	//get mousePos
	if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_RB)) {
		LOG_INFO("CursorPosition:{} {}", GetCursorPosition().x+m_CameraWorldCoord.translation.x, GetCursorPosition().y + m_CameraWorldCoord.translation.y);
		//LOG_INFO("PlayerPosition:{} {}", m_Transform.translation.x, m_Transform.translation.y);
	}
}


