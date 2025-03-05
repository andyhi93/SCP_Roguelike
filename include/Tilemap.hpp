#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Collider.hpp"
#include "Door.hpp"

class Tilemap : public Util::GameObject {
public:
    Tilemap();
    void Init();

    void Update();
    void SetDoors(bool east, bool south , bool west, bool north);

    enum RoomType {
        //func
        StartRoom,
        shop,
        treasureRoom,
        BossRoom,
        //easy
        Room610,
        Room553,
        Room743ant,
        Room1048B,
        Room1048C,
        Room049_2,
        //medium
        Room1048B_1048C,
        Room553_610,
        Room610_049_2,
        Room3119,
        //hard
        Room1048_610,
        Room3119_553,
        Room049_2_1048,
        Room553_610_743ant,
    };
    struct RoomSetStru {
        std::vector<std::shared_ptr<GameObject>> enemyObjs;
        std::vector<glm::vec2> objPos;
    };
    RoomSetStru roomSet[RoomType::Room553_610_743ant];
    std::vector<std::shared_ptr<Door>> doors;
private:
    std::vector<std::string> roomImages;
    bool IsInit = false;
    bool hasDoor[4] = { false, false, false, false };//ESWN
};

#endif
