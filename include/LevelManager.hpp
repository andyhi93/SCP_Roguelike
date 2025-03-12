#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "pch.hpp"
#include "Tilemap.hpp"
#include "core/Object.hpp"
#include "UI.hpp"
#include "Enemy.hpp"

class Player;
constexpr int MAP_SIZE_WIDTH = 5;
constexpr int MAP_SIZE_HEIGHT = 6;
constexpr int MIN_ROOMS = 7;
constexpr int MAX_ROOMS = 10;

class LevelManager : public Object {
public:
    LevelManager();
    void GenerateLevel();
    void PrintMap();
    void ChangeRoom(glm::ivec2 direction);

    std::shared_ptr<Tilemap> m_Tilemap = std::make_shared<Tilemap>();
    std::shared_ptr<UI> m_MapUI;
    std::vector<std::shared_ptr<Object>> currentObjects = {};

    void setPlayer(std::shared_ptr<Player> _player);

    void Update();
    void FixedUpdate() override;

private:

    UI::Room map[MAP_SIZE_WIDTH][MAP_SIZE_HEIGHT];
    glm::ivec2 startPos;
    glm::ivec2 bossPos;

    int roomCount = 0;
    int floor = 0;
    glm::ivec2 currentRoom = { 0,0 };

    bool IsValidRoom(int x, int y);
    void AddRoom(int x, int y);

    std::shared_ptr<Player> m_Player;

};

#endif
