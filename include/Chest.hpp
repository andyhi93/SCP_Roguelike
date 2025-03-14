#ifndef CHEST_HPP
#define CHEST_HPP

#include "Item.hpp"
#include "Core/Solid.hpp"

class Chest : public Solid {
public:
    Chest(glm::vec2 pos, glm::vec2 size);

    void Update();
    void Open();

    bool isOpen = false;
private:
	std::shared_ptr<Item> treasure;
    std::shared_ptr<Actor> openArea;
};

#endif
