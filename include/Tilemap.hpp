#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "pch.hpp"

#include "core/Object.hpp"
#include "Door.hpp"

class Enemy;
class Tilemap : public Object {
public:
    Tilemap();
    void Init();

    void Update();
    void SetDoors(bool east, bool south , bool west, bool north);

    enum RoomType {
        //func
        StartRoom, shop, treasureRoom, BossRoom,
        //easy
        Room610, Room553, Room743ant, Room1048, Room049_2,
        //medium
        Room553_610, Room610_049_2, Room1048_743, Room1048_049_2,
        //Room3119,
        //hard
        Room1048_610, Room3119_553, Room049_2_1048, Room553_610_743ant,
    };
    enum Building {
        Table8, Table4, Table3, Table2, Trap10 , TrapRec, TrapCol4
    };
    RoomType seRoom[8] = { StartRoom ,BossRoom,Room610,Room049_2,Room553_610 ,Room1048_743,Room1048_610,Room553_610_743ant };
    Building BuildingLast = TrapCol4;
    std::vector<std::shared_ptr<Object>> InitRoom(RoomType _RoomType,int entrancePos,int maxEnemyAmount);
    std::vector<std::shared_ptr<Door>> doors;
    bool hasDoor[4] = { false, false, false, false };//ESWN
private:
    std::vector<std::string> roomImages;
    bool IsInit = false;
};

#endif
