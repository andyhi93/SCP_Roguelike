#include "Core/Camera.hpp"
#include <iostream>

Camera::Camera(std::vector<std::weak_ptr<Object>> pivotChildren) : m_RelativePivotChildren(pivotChildren){
	m_CameraWorldCoord.translation = { 0.0f,0.0f };
}
void Camera::CameraFollowWith(glm::vec2 target) { m_CameraWorldCoord.translation = target; }

void Camera::AddRelativePivotChild(std::weak_ptr<Object> child) {
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
		if (auto lockedChild = child.lock()) {  // �T�O weak_ptr ���M����
			lockedChild->m_Transform.translation = lockedChild->m_WorldCoord - m_CameraWorldCoord.translation;
			if(lockedChild->layer== Object::CollisionLayer::Player) std::cout << "pos:" << (lockedChild->m_WorldCoord - m_CameraWorldCoord.translation).x << " ," << (lockedChild->m_WorldCoord - m_CameraWorldCoord.translation).y << "\n";
		}
	}
}


