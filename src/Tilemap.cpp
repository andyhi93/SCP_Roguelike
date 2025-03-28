#include "Tilemap.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include "Util/Image.hpp"
#include <iostream>
#include <cmath>
#include "Enemy.hpp"
#include "Enemies/SCP610.hpp"
#include "Enemies/SCP049_2.hpp"
#include "Enemies/SCP743A.hpp"
#include "Enemies/SCP743B.hpp"
#include "Enemies/SCP1048_B.hpp"
#include "Enemies/SCP1048_C.hpp"
#include "Enemies/SCP553.hpp"
#include "Enemies/SCP049.hpp"
#include "Enemies/SCP743.hpp"
#include "Table.hpp"
#include "Trap.hpp"
#include "Item.hpp"
#include "Chest.hpp"
#include "Elevator.hpp"
#include <random>

Tilemap::Tilemap() {
    m_Transform.translation = { 0, 0 };
    m_Transform.scale = { 7, 7 };
}
std::vector<std::shared_ptr<Object>> Tilemap::InitRoom(RoomType _RoomType, int entrancePos, int maxEnemyAmount) {
    std::vector<std::vector<glm::vec2>> wallData = { {glm::vec2(845, -303), glm::vec2(10, 335)},{glm::vec2(845, 263), glm::vec2(10, 538)},
        {glm::vec2(-485, -475), glm::vec2(830, 10)},{glm::vec2(475, -475), glm::vec2(830, 10)},{glm::vec2(-870, -303), glm::vec2(60, 335)},{glm::vec2(-870, 263), glm::vec2(63, 538)},
        {glm::vec2(-485, 425), glm::vec2(830, 100)},{glm::vec2(440, 425), glm::vec2(770, 100)}, };
    for (int i = 0; i < 8; i++) {
        std::shared_ptr<Solid> tempWall = std::make_shared<Solid>(wallData[i][0], wallData[i][1]);
        tempWall->m_collider->tag = "Wall";
        tempWall->m_collider->isSolid = true;
        tempWall->Start();
        mobWalls.push_back(tempWall);
    }


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
    if (_RoomType == StartRoom) {
        auto elevator = std::make_shared<Elevator>(glm::vec2{ -400,385 }, glm::vec2{100,100});
        elevator->SetZIndex(this->GetZIndex() + 0.1f);
        elevator->SetActive(false);
        elevator->isStartRoom = true;
        Obj.push_back(elevator);
    }
    if (_RoomType == BossRoom) {
        auto elevator = std::make_shared<Elevator>(glm::vec2{ -400,385 }, glm::vec2{ 100,100 });
        elevator->SetZIndex(this->GetZIndex() + 0.1f);
        elevator->SetActive(false);
        Obj.push_back(elevator);
        elevator->isBossRoom = true;
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
void Tilemap::InitDoor() {
    std::vector<glm::vec2> doorPos = { glm::vec2 {870, -70} ,glm::vec2 {-7 ,-500} ,glm::vec2 {-870 ,-70 },glm::vec2 {-7, 390} };
    std::vector<glm::vec2> doorSize = { glm::vec2 {60, 120} ,glm::vec2 {126, 57} ,glm::vec2 {60, 123},glm::vec2 {126, 150} };
    std::string path = "";
    for (int i = 0; i < 4; i++) {
        std::shared_ptr<Door> temp_door = std::make_shared<Door>(doorPos[i],doorSize[i]);
        //temp_door->Start();
        temp_door->m_collider->tag = "Door"+ std::to_string(i);
        if (i == 3) {
            temp_door->m_collider->offset = { 0,60 };
            temp_door->m_Transform.translation = doorPos[i];
            temp_door->m_collider->position = temp_door->m_collider->offset + temp_door->m_Transform.translation;
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
    roomImages = { RESOURCE_DIR "/Room/room_0000.png" ,RESOURCE_DIR "/Room/room_0001.png"
    ,RESOURCE_DIR "/Room/room_0010.png",RESOURCE_DIR "/Room/room_0011.png"
    ,RESOURCE_DIR "/Room/room_0100.png" ,RESOURCE_DIR "/Room/room_0101.png"
    ,RESOURCE_DIR "/Room/room_0110.png" ,RESOURCE_DIR "/Room/room_0111.png"
    ,RESOURCE_DIR "/Room/room_1000.png" ,RESOURCE_DIR "/Room/room_1001.png"
    ,RESOURCE_DIR "/Room/room_1010.png" ,RESOURCE_DIR "/Room/room_1011.png"
    ,RESOURCE_DIR "/Room/room_1100.png" ,RESOURCE_DIR "/Room/room_1101.png"
    ,RESOURCE_DIR "/Room/room_1110.png" ,RESOURCE_DIR "/Room/room_1111.png" };
    this->SetDrawable(std::make_shared<Util::Image>(roomImages[std::stoi(path, nullptr, 2)]));
}
void Tilemap::SetDoors(bool east, bool south, bool west, bool north) {
    hasDoor[0] = east;
    hasDoor[1] = south;
    hasDoor[2] = west;
    hasDoor[3] = north;
    if (!IsInit) {
        InitDoor();
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

std::vector<std::shared_ptr<Object>> Tilemap::InitBossRoom(BossType _BossType){
    std::vector<std::shared_ptr<Object>> objs = {};
    std::vector<std::vector<glm::vec2>> wallData = { {{-410,-75},{49,492}},{{400,-76},{49,494}},{{-210,171},{318,165}},{{210,170},{310,167}},{{0,-300},{700,50}}, //BOTTOM L R TL TR B
        {{-75,464},{50,470}},{{80,464},{50,470}},//Bottom pipe L&R
        {{-650,700},{1200,50}},{{655,700},{1200,50}},{{-1200,1246},{62,1176}},{{1200,1246},{62,1176}},{{-650,1730},{1176,142}},{{630,1730},{1176,142}}, //BL BR L R TL TR
        {{-130,2000},{138,544}},{{110,2000},{138,544}}, //Top pipe L&R
        {{-315,2270},{510,60}},{{295,2270},{510,60}},{{-540,2750},{64,838}},{{525,2750},{64,838}},{{0,2985},{1130,180}} //TOP BL BR L R T
    };
    for (int i = 0; i < wallData.size(); i++) {
        std::shared_ptr<Solid> tempWall = std::make_shared<Solid>(wallData[i][0], wallData[i][1]);
        tempWall->m_collider->tag = "Wall";
        tempWall->m_collider->isSolid = true;
        tempWall->Start();
        bossWalls.push_back(tempWall);
    }

    std::vector<std::vector<glm::vec2>> doorData = { {{0,690},{60,69}} ,{{0,1685},{126, 150}} };
    for (int i = 0; i < 2; i++) {
        std::shared_ptr<Door> temp_door = std::make_shared<Door>(doorData[i][0], doorData[i][1]);
        temp_door->m_collider->tag = "Door" + std::to_string(i);
        if (i == 1) {
            temp_door->m_collider->offset = { 0,50 };
            temp_door->m_collider->position = temp_door->m_collider->offset + temp_door->m_Transform.translation;
        }
        temp_door->Start();
        temp_door->m_collider->isSolid = true;
        temp_door->SetZIndex(this->GetZIndex() + 0.1f);
        this->AddChild(temp_door);
        doors.push_back(temp_door);
        if (i == 1) temp_door->SetIsTop(true);
        else {
            temp_door->m_Transform.rotation = (i+1 * 90) * (M_PI / 180.0);
        }
        temp_door->SetDrawable(std::make_shared<Util::Image>(temp_door->doorImage[0]));
    }
    doors[0]->DoorControl(true);
    doors[1]->DoorControl(true);

    this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/Room/BossRoom.png"));
    m_WorldCoord= { 0,1380 };
    m_Transform.translation = { 0,1380 };


    std::shared_ptr<Enemy> Boss;
    if(_BossType==RoomSCP049) Boss = std::make_shared<SCP049>();
    else Boss = std::make_shared<SCP743>();
    Boss->Start();
    Boss->m_Transform.translation = { 0,1500 };
    Boss->m_WorldCoord = { 0,1500 };
    Boss->SetZIndex(this->GetZIndex() + 0.5f);
    Boss->isCameraOn = true;
    this->AddChild(Boss);
    objs.push_back(Boss);

    auto chest = std::make_shared<Chest>(glm::vec2{ 0,2576 }, glm::vec2{ 150,100 });
    chest->isCameraOn = true;
    chest->m_WorldCoord = glm::vec2{ 0,2576 };
    chest->SetZIndex(this->GetZIndex() + 0.1f);
    chest->SetActive(true);
    this->AddChild(chest);
    chest->Start();
    objs.push_back(chest);

    auto redPill = std::make_shared<Item>(glm::vec2{ -250,2576 },Item::ItemType::bloodPill);
    redPill->isCameraOn = true;
    redPill->m_WorldCoord = glm::vec2{ -250,2576 };
    redPill->SetZIndex(this->GetZIndex() + 0.1f);
    this->AddChild(redPill);
    redPill->Start();
    objs.push_back(redPill);

    auto elevator = std::make_shared<Elevator>(glm::vec2{ -239,2907 }, glm::vec2{ 100,100 });
    elevator->m_WorldCoord = glm::vec2{ -239,2915 };
    elevator->isCameraOn = true;
    elevator->SetZIndex(this->GetZIndex() + 0.1f);
    elevator->SetActive(true);
    this->AddChild(elevator);
    objs.push_back(elevator);
    return objs;
}