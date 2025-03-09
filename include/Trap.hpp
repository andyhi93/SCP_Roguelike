#ifndef TRAP_HPP
#define TRAP_HPP

#include "core/Solid.hpp"

class Trap : public Solid {
public:
	Trap(glm::vec2 pos, glm::vec2 size);
	void Update();
	void Start();
	void SetActive();
private:
	std::vector<std::string> trapImages;
};

#endif