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
};

#endif