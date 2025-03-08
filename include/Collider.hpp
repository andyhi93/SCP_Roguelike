#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include <glm/glm.hpp>
#include "pch.hpp"
class LevelManager;
class Collider  {
public:
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 offset = {0,0};

    Collider(glm::vec2 _pos, glm::vec2 _size);
    static const std::vector<Collider>& GetWalls();
    static const std::vector<Collider>& GetDoors();

    bool isStatic;

private:
    static std::vector<Collider> walls;
    static std::vector<Collider> doors;
};

#endif