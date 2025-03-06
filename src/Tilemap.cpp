#include "Tilemap.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include <cmath>
#include "Enemy.hpp"
#include "SCP610.hpp"
#include "SCP049_2.hpp"

Tilemap::Tilemap() {
    roomImages = { "../../../Resources/Room/room_0000.png" ,"../../../Resources/Room/room_0001.png" 
    ,"../../../Resources/Room/room_0010.png","../../../Resources/Room/room_0011.png"
    ,"../../../Resources/Room/room_0100.png" ,"../../../Resources/Room/room_0101.png" 
    ,"../../../Resources/Room/room_0110.png" ,"../../../Resources/Room/room_0111.png" 
    ,"../../../Resources/Room/room_1000.png" ,"../../../Resources/Room/room_1001.png" 
    ,"../../../Resources/Room/room_1010.png" ,"../../../Resources/Room/room_1011.png" 
    ,"../../../Resources/Room/room_1100.png" ,"../../../Resources/Room/room_1101.png" 
    ,"../../../Resources/Room/room_1110.png" ,"../../../Resources/Room/room_1111.png" };
    m_Transform.translation = { 0, 0 };
    m_Transform.scale = { 7, 7 };
}
std::vector<std::shared_ptr<Enemy>> Tilemap::InitRoom(RoomType _RoomType) {
    std::vector<std::shared_ptr<Enemy>> Objs;
    std::vector<glm::vec2> objPos;
    if (_RoomType == Room610) {
        //std::cout<<"init Room610" << std::endl;
        objPos = { {776, -365},{ -784,-389 }, { -776,338 }, { 776, 336 } };
        for (int i = 0; i < 4; i++) {
            Objs.push_back(std::make_shared<SCP610>());
        }
        //std::cout << "insize of Ojbs: " << Objs.size() <<std::endl;
        int i = 0;
        for (auto& enemy : Objs) {
            enemy->SetZIndex(this->GetZIndex() + 1);
            enemy->m_Transform.translation = objPos[i++];
        }
        return Objs;
    }
    if (_RoomType == Room049_2) {
        objPos = { {776, -365},{ -784,-389 }, { -776,338 }, { 776, 336 } };
        for (int i = 0; i < 4; i++) {
            Objs.push_back(std::make_shared<SCP049_2>());
        }
        int i = 0;
        for (auto& enemy : Objs) {
            enemy->SetZIndex(this->GetZIndex() + 1);
            enemy->m_Transform.translation = objPos[i++];
        }
        return Objs;
    }
    Objs = {};
    return Objs;
}

void Tilemap::Update() {
}
void Tilemap::Init() {
    std::vector<glm::vec2> doorPos = { glm::vec2 {870, -70} ,glm::vec2 {-7 ,-500} ,glm::vec2 {-870 ,-70 },glm::vec2 {-7, 390} };
    std::string path = "";
    for (int i = 0; i < 4; i++) {
        std::shared_ptr<Door> temp_door = std::make_shared<Door>();
        temp_door->m_Transform.translation = doorPos[i];
        temp_door->SetZIndex(this->GetZIndex() + 1);
        this->AddChild(temp_door);
        doors.push_back(temp_door);
        if (i == 3) temp_door->SetIsTop(true);
        else {
            temp_door->m_Transform.rotation = (i * 90) * (M_PI / 180.0);
        }
        temp_door->SetDrawable(std::make_shared<Util::Image>(temp_door->doorImage[0]));
        if (hasDoor[i]) {
            path += "1";
            temp_door->SetVisible(true);
        }
        else {
            path += "0";
            temp_door->SetVisible(false);
        }
    }
    this->SetDrawable(std::make_shared<Util::Image>(roomImages[std::stoi(path, nullptr, 2)]));
}
void Tilemap::SetDoors(bool east, bool south, bool west, bool north) {
    hasDoor[0] = east;
    hasDoor[1] = south;
    hasDoor[2] = west;
    hasDoor[3] = north;
    if (!IsInit) {
        Init();
        IsInit = true;
    }
    else {
        std::string path = "";
        for (int i = 0; i < 4; i++) {
            doors[i]->DoorControl(false);
            if (hasDoor[i]) {
                path += "1";
                doors[i]->SetVisible(true);
            }
            else {
                path += "0";
                doors[i]->SetVisible(false);
            }
        }
        this->SetDrawable(std::make_shared<Util::Image>(roomImages[std::stoi(path, nullptr, 2)]));
    }
}
