#ifndef DOOR_HPP
#define DOOR_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Core/Solid.hpp"

class Door: public Solid {
public:

    Door();

    void Update();

    void SetIsTop(bool _isTop);
    std::vector<std::string> doorImage;
    void DoorControl(bool cmd);
    bool isOpen = false;
protected:
private:
    bool isTop = false;
};

#endif
