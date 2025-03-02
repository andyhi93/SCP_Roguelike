#include "MapUI.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <iostream>

MapUI::MapUI() {
    this->AddChild(Background);
    m_Transform.translation = { 637 ,330 };
    Background->SetZIndex(10);
    Background->m_Transform.translation = { 637 ,330 };
    Background->SetDrawable(std::make_shared<Util::Image>("../../../Resources/Map/map_background.png"));
    Background->m_Transform.scale = { 6,6 };
    std::cout << "Background zindex:" << Background->GetZIndex() << std::endl;
    roomImages = { "../../../Resources/Map/map1.png" ,"../../../Resources/Map/map2.png" , 
    "../../../Resources/Map/map3.png" ,"../../../Resources/Map/map4.png" ,
    "../../../Resources/Map/map5.png" ,"../../../Resources/Map/map6.png" ,
    "../../../Resources/Map/map7.png" ,"../../../Resources/Map/map8.png" ,
    "../../../Resources/Map/map9.png" ,"../../../Resources/Map/map10.png" ,
    "../../../Resources/Map/map11.png" ,"../../../Resources/Map/map12.png" ,
    "../../../Resources/Map/map13.png" ,"../../../Resources/Map/map14.png" ,
    "../../../Resources/Map/map15.png" ,"../../../Resources/Map/map16.png" ,
    };
    colorImages = {
        "../../../Resources/Map/map_spawn.png",    
        "../../../Resources/Map/map_store.png" ,
        "../../../Resources/Map/map_treasure.png",
        "../../../Resources/Map/map_boss.png",   
    };
}
void MapUI::Init(std::vector<Room> _RoomData) {
    int roomIndex = 0;
    if (_RoomData.size() != 9) std::cout << "MapUI Init Wrong: RoomSize isn't 9!" << std::endl;
    for(int y=0;y<3;y++){
        for (int x = 0; x < 3; x++) {
            std::shared_ptr<GameObject> roomObject = std::make_shared<GameObject>();
            std::shared_ptr<GameObject> colorObject = std::make_shared<GameObject>();
            roomObject->SetZIndex(10.1f);
            roomObject->m_Transform.scale = { 6,6 };
            colorObject->SetZIndex(10.2f);
            colorObject->m_Transform.scale = { 6,6 };
            this->AddChild(roomObject);
            this->AddChild(colorObject);
            roomShape.push_back(roomObject);
            roomColor.push_back(colorObject);
            mapArray[x][y] = _RoomData[roomIndex++];//from left bottom
            if (mapArray[x][y].exists) {
                std::string path = "";
                for (int i = 0; i < 4; i++) {
                    if (mapArray[x][y].doors[i]) {
                        path = "1"+ path;
                    }
                    else {
                        path = "0"+ path;
                    }
                    roomObject->SetDrawable(std::make_shared<Util::Image>(roomImages[std::stoi(path, nullptr, 2)]));
                }
                if (mapArray[x][y].roomType < 4) {
                    colorObject->SetDrawable(std::make_shared<Util::Image>(colorImages[mapArray[x][y].roomType]));
                    roomColor.push_back(colorObject);
                }
            }
            glm::vec2 position = { m_Transform.translation.x+ (x-1) * 110,m_Transform.translation.y+ (y-1) * 64 };  // 假設每個房間間距 64 像素
            roomObject->m_Transform.translation = position;
            colorObject->m_Transform.translation = position;
        }
    }
}
void MapUI::SetMap(std::vector<Room> _RoomData, glm::vec2 direction) {
    if (!isInit) {
        isInit = true;
        Init(_RoomData);
    }
    else {
        if (direction.y > 0) {
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if(y<2){
                        mapArray[x][y] = mapArray[x][y + 1];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                    else {
                        mapArray[x][y] = _RoomData[x];
                        UpdateRoomDisplay(mapArray[x][y],x, y); 
                    }
                }
            }
        }
        else if (direction.y < 0) {
            for (int y = 2; y >-1; y--) {
                for (int x = 0; x < 3; x++) {
                    if (y > 0) {
                        mapArray[x][y] = mapArray[x][y -1];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                    else {
                        mapArray[x][y] = _RoomData[x];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                }
            }
        }
        else if (direction.x < 0) {
            for (int x = 2; x >-1; x--) {
                for (int y = 0; y < 3; y++) {
                    if (x>0) {
                        mapArray[x][y] = mapArray[x-1][y];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                    else {
                        mapArray[x][y] = _RoomData[x];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                }
            }
        }
        else if (direction.x > 0) {
            for (int x = 0; x < 3; x++) {
                for (int y = 0; y < 3; y++) {
                    if (x<2) {
                        mapArray[x][y] = mapArray[x + 1][y];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                    else {
                        mapArray[x][y] = _RoomData[x];
                        UpdateRoomDisplay(mapArray[x][y], x, y);
                    }
                }
            }
        }
    }
}

// 更新房間顯示
void MapUI::UpdateRoomDisplay(Room roomData,int x, int y) {
    std::shared_ptr<GameObject> roomObject = roomShape[x + y * 3];
    std::shared_ptr<GameObject> colorObject = roomColor[x + y * 3];

    if (roomData.exists) {
        // 根據房間門的開關來選擇顯示圖像
        std::string path = "";
        for (int i = 0; i < 4; i++) {
            path = (roomData.doors[i] ? "1" : "0") + path;
        }
        roomObject->SetDrawable(std::make_shared<Util::Image>(roomImages[std::stoi(path, nullptr, 2)]));

        if (roomData.roomType < 4) {
            colorObject->SetDrawable(std::make_shared<Util::Image>(colorImages[roomData.roomType]));
        }
        else {
            colorObject->SetDrawable(nullptr);
        }
    }
    else {
        // 處理該房間是空的情況
        roomObject->SetDrawable(nullptr);
        colorObject->SetDrawable(nullptr);
    }
}
void MapUI::Update() {
}
