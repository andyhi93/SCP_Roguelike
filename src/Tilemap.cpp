#include "Tilemap.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include <cmath>
#include "Enemy.hpp"
#include "SCP610.hpp"
#include "SCP049_2.hpp"
#include "SCP743A.hpp"
#include "SCP743B.hpp"
#include "SCP1048_B.hpp"
#include "SCP1048_C.hpp"
#include "SCP553.hpp"
#include "Table.hpp"
#include "Trap.hpp"
#include "Item.hpp"
#include "Chest.hpp"
#include <random>

Tilemap::Tilemap() {
    roomImages = { RESOURCE_DIR "/Room/room_0000.png" ,RESOURCE_DIR "/Room/room_0001.png"
    ,RESOURCE_DIR "/Room/room_0010.png",RESOURCE_DIR "/Room/room_0011.png"
    ,RESOURCE_DIR "/Room/room_0100.png" ,RESOURCE_DIR "/Room/room_0101.png"
    ,RESOURCE_DIR "/Room/room_0110.png" ,RESOURCE_DIR "/Room/room_0111.png"
    ,RESOURCE_DIR "/Room/room_1000.png" ,RESOURCE_DIR "/Room/room_1001.png"
    ,RESOURCE_DIR "/Room/room_1010.png" ,RESOURCE_DIR "/Room/room_1011.png"
    ,RESOURCE_DIR "/Room/room_1100.png" ,RESOURCE_DIR "/Room/room_1101.png"
    ,RESOURCE_DIR "/Room/room_1110.png" ,RESOURCE_DIR "/Room/room_1111.png" };
    m_Transform.translation = { 0, 0 };
    m_Transform.scale = { 7, 7 };
}
std::vector<std::shared_ptr<Object>> Tilemap::InitRoom(RoomType _RoomType, int entrancePos, int maxEnemyAmount) {
    std::vector<std::shared_ptr<Enemy>> EnemyObjs;
    std::vector<glm::vec2> EnemyObjPos;
    std::random_device rd;
    std::mt19937 gen(rd());
    int enemyCount = 0;
    if (_RoomType == Room610 || _RoomType == Room553_610 || _RoomType== Room610_049_2 || _RoomType== Room1048_610) {
        for (int i = 0; i < 4 && enemyCount<= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP610>());
            enemyCount++;
        }
    }
    if (_RoomType == Room049_2 || _RoomType == Room610_049_2 || _RoomType== Room1048_049_2) {
        for (int i = 0; i < 4 && enemyCount <= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP049_2>());
            enemyCount++;
        }
    }
    if (_RoomType == Room743ant || _RoomType== Room1048_743) {
        std::uniform_int_distribution<int> dis(0, 4);
        int rdnum = dis(gen);
        for (int i = 0; i < rdnum && enemyCount <= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP743A>());
            enemyCount++;
        }
        for (int i = 0; i < 4- rdnum && enemyCount <= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP743B>());
            enemyCount++;
        }
    }
    if (_RoomType == Room1048 || _RoomType== Room1048_743 || _RoomType== Room1048_049_2 || _RoomType== Room1048_610) {
        std::uniform_int_distribution<int> dis(0, 2);
        int rdnum = dis(gen);
        for (int i = 0; i < rdnum && enemyCount <= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP1048_B>());
            enemyCount++;
        }
        for (int i = 0; i < 2 - rdnum && enemyCount <= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP1048_C>());
            enemyCount++;
        }
    }
    if (_RoomType == Room553 || _RoomType == Room553_610) {
        maxEnemyAmount += 2;
        for (int i = 0; i < 6 && enemyCount <= maxEnemyAmount; i++) {
            EnemyObjs.push_back(std::make_shared<SCP553>());
            enemyCount++;
        }
    }
    int i = 0;
    if (entrancePos == 0 || entrancePos == 2) EnemyObjPos = { {275, 250},{ -55,250}, { -397,250 }, { -743, 250 },{275,-400},{-55,400},{-397,400},{-743,400} };
    if (entrancePos == 1 || entrancePos == 3) EnemyObjPos = { {-690, 168},{ -500,0}, { -690,-168 }, { -500, -238 },{610,166},{440,0},{610,-144},{440,-280} };
    std::shuffle(EnemyObjPos.begin(), EnemyObjPos.end(), rd);
    for (auto& enemy : EnemyObjs) {
        enemy->SetActive(false);
        enemy->SetZIndex(this->GetZIndex() + 0.5f);
        enemy->m_Transform.translation = EnemyObjPos[i];
        i = i > 6 ? 1 : i + 1;
    }
    if (EnemyObjs.empty()) { EnemyObjs = {}; }

    std::vector<std::shared_ptr<Object>> Obj;
    if (_RoomType != StartRoom && _RoomType != treasureRoom && _RoomType != shop && _RoomType != BossRoom) {
        std::uniform_int_distribution<int> dis(0, BuildingLast);
        Building rdBuilding = (Building)dis(gen);
        //Building rdBuilding = TrapRec;
        std::vector<glm::vec2> BuildingObjPos;
        if (rdBuilding == Table8 || rdBuilding == Table4 || rdBuilding == Table3 || rdBuilding == Table2) {
            if (rdBuilding == Table8) BuildingObjPos = { { -210,160}, {210,160}, { -210,-280}, {210,-280},{-625,160},{ 625,160},{-625,-280},{ 625,-280} };
            if (rdBuilding == Table4) BuildingObjPos = { { -210,160}, {210,160}, { -210,-280}, {210,-280} };
            if (rdBuilding == Table3) BuildingObjPos = { { -0,160}, {0,-60}, { -0,-280} };
            if (rdBuilding == Table2) BuildingObjPos = { { -300, 0 }, { 300,0 } };
            for (int i = 0; i < BuildingObjPos.size(); i++) {
                std::shared_ptr<Table> table = std::make_shared<Table>(BuildingObjPos[i], glm::vec2{ 250,80 });
                table->Start();
                table->m_collider->isActive = false;
                table->m_collider->SetOffset({ 0,20 });
                table->SetZIndex(this->GetZIndex() + 0.4f);
                Obj.push_back(table);
            }
        }
        if (rdBuilding == Trap10 || rdBuilding == TrapRec || rdBuilding == TrapCol4) {
            if (rdBuilding == Trap10) { BuildingObjPos = { { 0,-66},{ 150,-66},{ 300,-66},{ 450,-66},{ 600,-66},{ -150,-66},{ -300,-66},{ -450,-66},{ -600,-66},
                { 0,0},{ 0,66} ,{ 0,132 } ,{ 0,198 } ,{ 0,-132 } ,{ 0,-198 },{ 0,-264 } ,{0,-330} };
            }
            if (rdBuilding == TrapRec) {BuildingObjPos = { { 0,-230},{ 150,-230},{ 300,-230},{ 450,-230},{ 600,-230},{ -150,-230},{ -300,-230},{ -450,-230},{ -600,-230},
            { 0,166},{ 150,166},{ 300,166},{ 450,166},{ 600,166},{ -150,166},{ -300,166},{ -450,166},{ -600,166},
                {-600,-164},{-600,-98},{-600,-32},{-600,34},{-600,100},{-600,166},
            {600,-164},{600,-98},{600,-32},{600,34},{600,100},{600,166}, };
            }
            if (rdBuilding == TrapCol4) { BuildingObjPos = { { 150,198 } ,{ 150,132 } ,{ 150,66},{ 150,0}  ,{ 150,-66} ,{ 150,-132 } ,{ 150,-198 },{ 150,-264 },{ 150,-330 },
            { 450,198 } ,{ 450,132 } ,{ 450,66},{ 450,0}  ,{ 450,-66} ,{ 450,-132 } ,{ 450,-198 },{ 450,-264 },{ 450,-330 },
            { -450,198 } ,{ -450,132 } ,{ -450,66},{ -450,0}  ,{ -450,-66} ,{ -450,-132 } ,{ -450,-198 },{ -450,-264 } ,{ -450,-330 } ,
            { -150,198 } ,{ -150,132 } ,{ -150,66},{ -150,0}  ,{ -150,-66} ,{ -150,-132 } ,{ -150,-198 },{ -150,-264 },{ -150,-330 } }; }
            for (int i = 0; i < BuildingObjPos.size(); i++) {
                std::shared_ptr<Trap> trap = std::make_shared<Trap>(BuildingObjPos[i], glm::vec2{ 125,40 });
                trap->Start();
                trap->m_collider->isActive = false;
                trap->m_collider->SetOffset({ 0,20 });
                trap->SetZIndex(this->GetZIndex() + 0.2f);
                Obj.push_back(trap);
            }
        }
    }
    if (_RoomType == shop) {
        std::unordered_set<int> generatedItems; 

        for (int i = 0; i < 3; i++) {
            std::uniform_int_distribution<int> dis(Item::bloodPill, Item::lastItemIndex);
            std::vector<glm::vec2> merchPos = { {450,0},{0,0},{-450,0} };
            Item::ItemType itemType;
            do {
                itemType = static_cast<Item::ItemType>(dis(gen));
            } while (generatedItems.find(static_cast<int>(itemType)) != generatedItems.end());  

            generatedItems.insert(static_cast<int>(itemType));

            auto merchItem = std::make_shared<Item>(merchPos[i], itemType);
            merchItem->SetZIndex(this->GetZIndex() + 1);
            merchItem->Start();
            merchItem->m_collider->isActive = false;
            merchItem->isUnlocked = false;
            merchItem->SetSell();
            Obj.push_back(merchItem);
        }
    }
    if (_RoomType == treasureRoom) {
        auto chest = std::make_shared<Chest>(glm::vec2{0,0},glm::vec2{150,100});
        chest->Start();
        chest->SetZIndex(this->GetZIndex() + 0.1f);
        chest->Start();
        chest->m_collider->isActive = false;
        Obj.push_back(chest);
    }
    for (auto& enemy : EnemyObjs) {
        Obj.push_back(enemy);
    }
    return Obj;
}

void Tilemap::Update() {
}
void Tilemap::Init() {
    std::vector<glm::vec2> doorPos = { glm::vec2 {870, -70} ,glm::vec2 {-7 ,-500} ,glm::vec2 {-870 ,-70 },glm::vec2 {-7, 450} };
    std::vector<glm::vec2> doorSize = { glm::vec2 {60, 120} ,glm::vec2 {126, 57} ,glm::vec2 {60, 123},glm::vec2 {126, 150} };
    std::string path = "";
    for (int i = 0; i < 4; i++) {
        std::shared_ptr<Door> temp_door = std::make_shared<Door>(doorPos[i],doorSize[i]);
        temp_door->m_collider->tag = "Door"+ std::to_string(i);
        if (i == 3) {
            temp_door->m_Transform.translation = doorPos[i] - glm::vec2{0, 60};
        }
        else {
            temp_door->m_Transform.translation = doorPos[i];
        }
        temp_door->m_collider->isSolid = true;
        temp_door->SetZIndex(this->GetZIndex() + 0.1f);
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
