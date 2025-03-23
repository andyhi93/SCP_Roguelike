#include "UI.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include "Player.hpp"
#include "UIText.hpp"
#include <iostream>

UI::UI(bool isBossRoom) {
    if (!isBossRoom) {
        this->AddChild(Background);
        m_Transform.translation = { 637 ,330 };
        Background->SetZIndex(10);
        Background->m_Transform.translation = { 637 ,330 };
        Background->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Map/map_background.png"));
        Background->m_Transform.scale = { 6,6 };

        this->AddChild(PlayerPoint);
        PlayerPoint->m_Transform.translation = { 637 ,330 };
        PlayerPoint->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Map/map_point.png"));
        PlayerPoint->m_Transform.scale = { 6,6 };
        PlayerPoint->SetZIndex(10.3);

        roomImages = { RESOURCE_DIR "/Map/map1.png" ,RESOURCE_DIR "/Map/map2.png" ,
        RESOURCE_DIR "/Map/map3.png" ,RESOURCE_DIR "/Map/map4.png" ,
        RESOURCE_DIR "/Map/map5.png" ,RESOURCE_DIR "/Map/map6.png" ,
        RESOURCE_DIR "/Map/map7.png" ,RESOURCE_DIR "/Map/map8.png" ,
        RESOURCE_DIR "/Map/map9.png" ,RESOURCE_DIR "/Map/map10.png" ,
        RESOURCE_DIR "/Map/map11.png" ,RESOURCE_DIR "/Map/map12.png" ,
        RESOURCE_DIR "/Map/map13.png" ,RESOURCE_DIR "/Map/map14.png" ,
        RESOURCE_DIR "/Map/map15.png" ,RESOURCE_DIR "/Map/map16.png" ,
        };
        colorImages = {
            RESOURCE_DIR "/Map/map_spawn.png",
            RESOURCE_DIR "/Map/map_store.png" ,
            RESOURCE_DIR "/Map/map_treasure.png",
            RESOURCE_DIR "/Map/map_boss.png",
        };
    }

    setHealthUI();
}
void UI::setHealthUI() {
    DashUIImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/DashUI1.png"));
    this->AddChild(DashUIImage);
    DashUIImage->m_Transform.scale = { 4,4 };
    DashUIImage->m_Transform.translation = { -520,428 };
    DashUIImage->SetZIndex(10.2f);

    HealthFrameImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/HealthFrame.png"));
    HealthBarImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/HealthBar.png"));
    this->AddChild(HealthFrameImage);
    HealthFrameImage->m_Transform.scale = { 3,3 };
    HealthFrameImage->m_Transform.translation = { -732,428 };
    HealthFrameImage->SetZIndex(10.2f);
    this->AddChild(HealthBarImage);
    HealthBarImage->m_Transform.scale = { 3,3 };
    HealthBarImage->m_Transform.translation = { -828,428 };
    HealthBarImage->SetZIndex(10);

    m_healthText = std::make_unique<UIText>(RESOURCE_DIR "/UI/PixelText.ttf", 50);
    m_healthText->SetZIndex(10.1f);
    m_healthText->Start();
    m_healthText->m_Transform.translation = { -710,428 };
    m_healthText->m_Transform.scale = { 0.7,0.7 };
    m_healthText->m_Text->SetText(fmt::format("{}/{}", currentHealth, maxHealth));
    this->AddChild(m_healthText);

    m_coinText = std::make_unique<UIText>(RESOURCE_DIR "/UI/PixelText.ttf", 50);
    m_coinText->SetZIndex(10.1f);
    m_coinText->Start();
    m_coinText->m_Transform.translation = { -800,355 };
    m_coinText->m_Transform.scale = { 1,1 };
    m_coinText->m_Text->SetText("0");
    this->AddChild(m_coinText);
}
void UI::InitMapUI(std::vector<Room> _RoomData) {
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
            if (mapArray[x][y].exists && mapArray[x][y].isVisible) {
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
        InitMapUI(_RoomData);
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

    if (roomData.exists && roomData.isVisible) {
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
void UI::SetPlayer (std::weak_ptr<Player> _player) { 
    MapPlayer = _player; 
    maxHealth= MapPlayer.lock()->GetHealth();
    currentHealth = MapPlayer.lock()->GetCurrentHealth();
}
void UI::Update() {
    if (PlayerPoint) {
        glm::vec2 zoomDiff = { 1631 / 79 ,650 / 30 };
        PlayerPoint->m_Transform.translation = m_Transform.translation + glm::vec2{ MapPlayer.lock()->m_Transform.translation.x / zoomDiff[0], MapPlayer.lock()->m_Transform.translation.y / zoomDiff[1] };
    }
    //HealthUI
    maxHealth = MapPlayer.lock()->GetHealth();
    currentHealth = MapPlayer.lock()->GetCurrentHealth();
    HealthBarImage->m_Transform.scale.x = (currentHealth<0)? 0: currentHealth / maxHealth*3;
    m_healthText->m_Text->SetText(fmt::format("{}/{}", (int)currentHealth, (int)maxHealth));
    //CoinText
    m_coinText->m_Text->SetText(std::to_string(MapPlayer.lock()->GetCoin()));

    if (MapPlayer.lock()->getCanDash() && !UICanDash) {
        UICanDash = true;
        DashUIImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/DashUI1.png"));
    }
    else if (!MapPlayer.lock()->getCanDash() && UICanDash) {
        UICanDash = false;
        DashUIImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/UI/DashUI2.png"));
    }
}
