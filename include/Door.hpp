#ifndef DOOR_HPP
#define DOOR_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Charater.hpp"
#include "Collider.hpp"

class Door: public Charater {
public:
    std::shared_ptr<Collider> m_Collider;

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
