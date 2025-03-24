#ifndef ELEVATOR_HPP
#define ELEVATOR_HPP

#include "Core/Actor.hpp"

class Elevator : public Actor {
public:
	Elevator(glm::vec2 pos, glm::vec2 size);
	void Update() override;
	void Start();
	void SetActive(bool active);
	bool isStartRoom = false;
	bool isBossRoom = false;
private:
};

#endif