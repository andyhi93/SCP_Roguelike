#ifndef TABLE_HPP
#define TABLE_HPP

#include "core/Solid.hpp"

class Player;
class Table: public Solid{
public:
	Table(glm::vec2 pos, glm::vec2 size);
	void Update();
	int tableIndex = 0;
	void BreakTable();
private:
	bool isBroken = false;
	std::vector<std::string> tableImages;
};

#endif