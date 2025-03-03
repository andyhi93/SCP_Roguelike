#ifndef MAPUI_HPP
#define MAPUI_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Util/Image.hpp"
class Player;

class MapUI : public Util::GameObject{
public:
    MapUI();

    struct Room {
        bool exists = false;
        bool doors[4] = { false, false, false, false };//ESWN
        int roomType;
    };
    void Update();
    void Init(std::vector<Room> _RoomData);
    void SetMap(std::vector<Room> _RoomData);
    void GetPlayer(std::shared_ptr<Player> _player);

    enum RoomType {
        //func
        StartRoom,
        shop,
        treasureRoom,
        BossRoom,
    };
    Room mapArray[3][3];
    std::shared_ptr<Player> MapPlayer = std::make_shared<Player>();
    std::shared_ptr<GameObject> Background=std::make_shared<GameObject>();
    std::shared_ptr<GameObject> PlayerPoint = std::make_shared<GameObject>();
    std::vector<std::shared_ptr<GameObject>> roomShape;
    std::vector<std::shared_ptr<GameObject>> roomColor;

private:
    bool isInit = false;
    std::vector<std::string> roomImages;
    std::vector<std::string> colorImages;
    void UpdateRoomDisplay(Room roomData,int x, int y);
};

#endif
