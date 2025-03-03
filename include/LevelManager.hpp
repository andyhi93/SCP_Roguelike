#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "pch.hpp"
#include "Tilemap.hpp"
#include "Util/GameObject.hpp"
#include "MapUI.hpp"

constexpr int MAP_SIZE_WIDTH = 5;
constexpr int MAP_SIZE_HEIGHT = 6;
constexpr int MIN_ROOMS = 7;
constexpr int MAX_ROOMS = 10;

class LevelManager : public Util::GameObject {
public:
    LevelManager();
    void GenerateLevel();
    void PrintMap();
    void ChangeRoom(glm::ivec2 direction);

    std::shared_ptr<Tilemap> m_Tilemap = std::make_shared<Tilemap>();
    std::shared_ptr<MapUI> m_MapUI;

    void Update();

private:

    MapUI::Room map[MAP_SIZE_WIDTH][MAP_SIZE_HEIGHT];
    glm::ivec2 startPos;
    glm::ivec2 bossPos;

    int roomCount = 0;
    int floor = 0;
    glm::ivec2 currentRoom = { 0,0 };

    bool IsValidRoom(int x, int y);
    void AddRoom(int x, int y);

};

#endif
