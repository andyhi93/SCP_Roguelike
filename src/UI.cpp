#include "UI.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include "Player.hpp"
#include "UIText.hpp"
#include <iostream>

UI::UI() {
    this->AddChild(Background);
    m_Transform.translation = { 637 ,330 };
    Background->SetZIndex(10);
    Background->m_Transform.translation = { 637 ,330 };
    Background->SetDrawable(std::make_shared<Util::Image>("../../../Resources/Map/map_background.png"));
    Background->m_Transform.scale = { 6,6 };

    //HealthUI
    HealthFrameImage->SetDrawable(std::make_shared<Util::Image>("../../../Resources/UI/HealthFrame.png"));
    HealthBarImage->SetDrawable(std::make_shared<Util::Image>("../../../Resources/UI/HealthBar.png"));
    this->AddChild(HealthFrameImage);
    HealthFrameImage->m_Transform.scale = { 3,3 };
    HealthFrameImage->m_Transform.translation = { -712,408 };
    HealthFrameImage->SetZIndex(10.2f);
    this->AddChild(HealthBarImage);
    HealthBarImage->m_Transform.scale = { 3,3 };
    HealthBarImage->m_Transform.translation = { -808,408 };
    HealthBarImage->SetZIndex(10);

    m_healthText = std::make_unique<UIText>("../../../Resources/UI/PixelText.ttf", 50);
    m_healthText->SetZIndex(10.1f);
    m_healthText->Start();
    m_healthText->m_Transform.translation = { -690,408 };
    m_healthText->m_Transform.scale = { 0.7,0.7 };
    m_healthText->m_Text->SetText(fmt::format("{}/{}", maxHealth, maxHealth));
    this->AddChild(m_healthText);

    this->AddChild(PlayerPoint);
    PlayerPoint->m_Transform.translation = { 637 ,330 };
    PlayerPoint->SetDrawable(std::make_shared<Util::Image>("../../../Resources/Map/map_point.png"));
    PlayerPoint->m_Transform.scale = { 6,6 };
    PlayerPoint->SetZIndex(10.3);

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
void UI::Init(std::vector<Room> _RoomData) {
    int roomIndex = 0;
    if (_RoomData.size() != 9) std::cout << "MapUI Init Wrong: RoomSize isn't 9!" << std::endl;
    for(int y=0;y<3;y++){
        for (int x = 0; x < 3; x++) {
            std::shared_ptr<Object> roomObject = std::make_shared<Object>();
            std::shared_ptr<Object> colorObject = std::make_shared<Object>();
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
                }
            }
            glm::vec2 position = { m_Transform.translation.x+ (x-1) * 110,m_Transform.translation.y+ (y-1) * 64 };  // 假設每個房間間距 64 像素
            roomObject->m_Transform.translation = position;
            colorObject->m_Transform.translation = position;
        }
    }
}
void UI::SetMap(std::vector<Room> _RoomData) {
    if (!isInit) {
        isInit = true;
        Init(_RoomData);
    }
    else {
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                mapArray[x][y] = _RoomData[x+y*3];
                UpdateRoomDisplay(mapArray[x][y], x, y);
            }
        }
    }
}

// 更新房間顯示
void UI::UpdateRoomDisplay(Room roomData,int x, int y) {
    std::shared_ptr<Object> roomObject = roomShape[x + y * 3];
    std::shared_ptr<Object> colorObject = roomColor[x + y * 3];

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
void UI::GetPlayer(std::shared_ptr<Player> _player) { 
    MapPlayer = _player; 
    maxHealth= MapPlayer->GetHealth();
    currentHealth = maxHealth;
}
void UI::Update() {
    glm::vec2 zoomDiff = { 1631 / 79 ,650 / 30 };
    PlayerPoint->m_Transform.translation = m_Transform.translation + glm::vec2{MapPlayer->m_Transform.translation.x/zoomDiff[0], MapPlayer->m_Transform.translation.y/zoomDiff[1] };

    //HealthUI
    currentHealth = MapPlayer->GetHealth();
    HealthBarImage->m_Transform.scale.x = (currentHealth<0)? 0: currentHealth / maxHealth*3;
    m_healthText->m_Text->SetText(fmt::format("{}/{}", currentHealth, maxHealth));
}
