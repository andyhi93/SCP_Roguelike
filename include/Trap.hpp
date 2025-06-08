#ifndef TRAP_HPP
#define TRAP_HPP

#include "core/Solid.hpp"

class Trap : public Solid, public Trigger {
public:
	Trap(glm::vec2 pos, glm::vec2 size);
	void Update();
	void Start() override;
	void SetActive();
	bool isOpen = false;
	bool isUp = false;
	void OnTriggerEnter(std::shared_ptr<BoxCollider> other) override;
private:
	std::vector<std::string> trapImages;
	float m_LastAttackTime = 0;
	float attackTime = 3;
	float attackSpeed = 7;
};

#endif