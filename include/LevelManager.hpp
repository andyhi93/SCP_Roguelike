#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "pch.hpp"
#include "Tilemap.hpp"
#include "core/Object.hpp"
#include "UI.hpp"
#include "Enemy.hpp"
#include "Core/Camera.hpp"

class Player;
constexpr int MAP_SIZE_WIDTH = 5;
constexpr int MAP_SIZE_HEIGHT = 6;
constexpr int MIN_ROOMS = 7;
constexpr int MAX_ROOMS = 10;

class LevelManager : public Object {
public:
    LevelManager(bool _isMobFloor,int floor);
    void GenerateLevel();
    void PrintMap();
    void ChangeRoom(glm::ivec2 direction);

    std::shared_ptr<Tilemap> m_Tilemap = std::make_shared<Tilemap>();
    std::shared_ptr<UI> m_UI;
    std::vector<std::shared_ptr<Object>> currentObjects = {};

    void setPlayer(std::weak_ptr<Player> _player);

    void Update();
    void FixedUpdate() override;

    void InitBossRoom();
    std::shared_ptr<Camera> m_Camera;
    int floor = 0;
    Tilemap::BossType bossType;
private:
    bool isMobFloor;
    bool isSummonBossB = false;

    UI::Room map[MAP_SIZE_WIDTH][MAP_SIZE_HEIGHT];
    glm::ivec2 startPos;
    glm::ivec2 bossPos;

    int roomCount = 0;
    glm::ivec2 currentRoom = { 0,0 };
    bool isOpenCurrentDoor = false;

    bool IsValidRoom(int x, int y);
    void AddRoom(int x, int y);

    std::weak_ptr<Player> m_Player;

    bool isEnterRoom = false;//BossRoom

    std::string m_openSFX = RESOURCE_DIR "/sound/OPENING THE DOOR.mp3";
    std::string m_closeSFX = RESOURCE_DIR "/sound/CLOSE THE DOOR.mp3";
};

#endif
