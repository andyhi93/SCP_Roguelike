#ifndef TRAP_HPP
#define TRAP_HPP

#include "core/Solid.hpp"

class Trap : public Solid {
public:
	Trap(glm::vec2 pos, glm::vec2 size);
	void Update();
	void Start() override;
	void SetActive();
	bool isOpen = false;
private:
	std::vector<std::string> trapImages;
	float m_LastAttackTime = 0;
	float attackTime = 3;
	float attackSpeed = 7;
};

#endif