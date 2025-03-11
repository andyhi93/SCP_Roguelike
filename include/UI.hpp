#ifndef MAPUI_HPP
#define MAPUI_HPP

#include "pch.hpp"

#include "core/Object.hpp"
#include "Util/Image.hpp"
#include "Enemy.hpp"
#include "Util/Text.hpp"
class Player;
class UIText;
class UI : public Object {
public:
    UI();

    struct Room {
        bool exists = false;
        bool doors[4] = { false, false, false, false };//ESWN
        int roomType;
        std::vector<std::shared_ptr<Object>> roomobjs = {};
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
    std::shared_ptr<Object> Background=std::make_shared<Object>();
    std::shared_ptr<Object> PlayerPoint = std::make_shared<Object>();
    std::vector<std::shared_ptr<Object>> roomShape;
    std::vector<std::shared_ptr<Object>> roomColor;

private:
    bool isInit = false;
    std::vector<std::string> roomImages;
    std::vector<std::string> colorImages;
    void UpdateRoomDisplay(Room roomData,int x, int y);

    std::shared_ptr<Object> HealthFrameImage = std::make_shared<Object>();
    std::shared_ptr<Object> HealthBarImage = std::make_shared<Object>();
    float currentHealth=0;
    float maxHealth=0; 
    std::string m_Font;
    int m_Size;
    std::shared_ptr<UIText> m_healthText;
};

#endif
