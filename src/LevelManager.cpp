#include "LevelManager.hpp"
#include <queue>
#include <random>
#include <iostream>
#include "Player.hpp"
#include "Enemies/SCP049.hpp"
#include "Elevator.hpp"
#include "Table.hpp"
#include <Trap.hpp>
#include "Item.hpp"
#include <Chest.hpp>

LevelManager::LevelManager(bool _isMobFloor,int floor) {
    this->floor = floor;
    isMobFloor = _isMobFloor;
    //std::cout << "isMobFloor:"<<isMobFloor<<"\n";
    if (isMobFloor) {
        m_UI = std::make_shared<UI>(false);
        this->AddChild(m_Tilemap);
        this->AddChild(m_UI);
        m_UI->SetZIndex(10);
        GenerateLevel();
    }
}
void LevelManager::InitBossRoom() {
    std::vector<std::weak_ptr<Object>> temp = { m_Player, m_Tilemap };
    m_Camera = std::make_shared<Camera>(temp);
    m_UI = std::make_shared<UI>(true);
    this->AddChild(m_UI);
    m_UI->SetZIndex(10);
    this->AddChild(m_Tilemap);
    auto objs = m_Tilemap->InitBossRoom(Tilemap::BossType::RoomSCP049);
    for (auto& obj : objs) {
        currentObjects.push_back(obj);
        m_Camera->AddRelativePivotChild(std::weak_ptr<Object>(obj));
    }
    m_UI->SetPlayer(m_Player);
    if (auto lockedChild = m_Player.lock()) {}
    else { std::cout << "nullptr Player\n"; }
    for (auto& wall : m_Tilemap->bossWalls) { m_Camera->AddRelativePivotChild(std::weak_ptr<Object>(wall)); }
    for (auto& door : m_Tilemap->doors) { m_Camera->AddRelativePivotChild(std::weak_ptr<Object>(door)); }
    auto boss = std::dynamic_pointer_cast<Enemy>(currentObjects[0]);
    boss->SetPlayer(m_Player);
    m_Player.lock()->isCameraOn = true;
}
void LevelManager::setPlayer(std::weak_ptr<Player> _player) {
    m_Player = _player;
}
bool LevelManager::IsValidRoom(int x, int y) {
    if (x < 0 || x >= MAP_SIZE_WIDTH || y < 0 || y >= MAP_SIZE_HEIGHT) return false;

    int neighborCount = 0;
    if (x > 0 && map[x - 1][y].exists ) neighborCount++;
    if (x < MAP_SIZE_WIDTH - 1 && map[x + 1][y].exists ) neighborCount++;
    if (y > 0 && map[x][y - 1].exists ) neighborCount++;
    if (y < MAP_SIZE_HEIGHT - 1 && map[x][y + 1].exists) neighborCount++;

    return (neighborCount <= 1);
}
void LevelManager::Update(){
    m_UI->Update();
    m_Tilemap->Update();
    if (!isMobFloor) { 
        if(!isEnterRoom && m_Player.lock()->m_WorldCoord.y > 800){
            std::cout << "=====================================enter room=====================================\n";
            isEnterRoom = true;
            for (auto& door : m_Tilemap->doors) door->DoorControl(false);
            isOpenCurrentDoor = false;
        }
        m_Camera->CameraFollowWith(m_Player.lock()->m_WorldCoord); 
        //std::cout << "camera worldcoord: " << m_Camera->GetCameraWorldCoord().translation.x << " ," << m_Camera->GetCameraWorldCoord().translation.y<<"\n"
        //    <<"player worldcoord: "<< m_Player.lock()->m_WorldCoord.x<<" ,"<< m_Player.lock()->m_WorldCoord.y<<"\n";
        m_Camera->Update();
        for (auto& bullet : m_Player.lock()->m_BulletBox->bullets) {
            if (!bullet->isInCamera) {
                bullet->isInCamera = true;
                m_Camera->AddRelativePivotChild(std::weak_ptr<Object>(bullet));
            }
        }

        std::vector<std::shared_ptr<Object>> tempObjects = currentObjects;
        for (auto& obj : tempObjects) {
            if (!obj) continue;
            auto boss = std::dynamic_pointer_cast<SCP049>(obj);
            if(!boss || (boss && isEnterRoom)) obj->Update();
            if (boss) {
                for (auto& bullet : boss->m_IRangedAttack->m_BulletBox->bullets) {
                    if (bullet && !bullet->isInCamera) {
                        bullet->isInCamera = true;
                        m_Camera->AddRelativePivotChild(std::weak_ptr<Object>(bullet));
                    }
                }
                if (boss->isSummon) {
                    auto mob = boss->summon();
                    if (mob) {
                        m_Camera->AddRelativePivotChild(std::weak_ptr<Object>(mob));
                        currentObjects.push_back(mob);  // 用另一個 vector 暫存
                    }
                }
            }
        }
    }
    //std::cout << "in fun(): sp.use_count() == " << m_MapUI.use_count()<< "\n";

    map[currentRoom.x][currentRoom.y].roomItems.erase(std::remove_if(map[currentRoom.x][currentRoom.y].roomItems.begin(), map[currentRoom.x][currentRoom.y].roomItems.end(),
        [this](const std::shared_ptr<Item>& item) {
            if (!item->hasDescripting && item->isPick || (item->hasDescripting && !item->isDescripting)) {
                this->RemoveChild(item);
                std::cout << "remove item\n";
                ColliderManager::GetInstance().UnregisterCollider(item->m_collider);
                return true;
            }
            return false;
        }),
        map[currentRoom.x][currentRoom.y].roomItems.end());
    if (!isMobFloor&&!isOpenCurrentDoor) {
        if (!map[currentRoom.x][currentRoom.y].isClean) {
            for (auto& obj : currentObjects) {
                std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
                if (enemy && !enemy->isDead) {
                    return;
                }
            }
        }
        for (auto& door : m_Tilemap->doors) {
            door->DoorControl(true);
        }
        isOpenCurrentDoor = true;
    }
    if (!isMobFloor) return;
    std::vector<std::shared_ptr<Object>> newCoins;
    for (auto& obj : currentObjects) {
        obj->Update();
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
        if (enemy && enemy->isDead && enemy->isDropCoin && !enemy->isGiveCoin) {
            auto coin = enemy->GetCoin();
            enemy->isGiveCoin = true;
            coin->Start();
            AddChild(coin);
            map[currentRoom.x][currentRoom.y].roomItems.push_back(coin);
            newCoins.push_back(coin);
        }
    }
    for (auto& coin : newCoins) {
        currentObjects.push_back(coin);
    }
    if (!isOpenCurrentDoor) {
        if (!map[currentRoom.x][currentRoom.y].isClean) {
            for (auto& obj : currentObjects) {
                std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
                if (enemy && !enemy->isDead) {
                    return;
                }
            }
        }
        if (isMobFloor) {
            map[currentRoom.x][currentRoom.y].isClean = true;
            if (map[currentRoom.x][currentRoom.y].isClean) {
                for (int i = 0; i < 4; i++) {
                    if (m_Tilemap->hasDoor[i]) m_Tilemap->doors[i]->DoorControl(true);
                }
            }
            else {
                for (auto& door : m_Tilemap->doors) {
                    door->DoorControl(true);
                }
            }
        }
        isOpenCurrentDoor = true;
    }
}
void LevelManager::FixedUpdate() {
    for (auto& obj : currentObjects) {
        obj->FixedUpdate();
    }
}

void LevelManager::AddRoom(int x, int y) {
    if (!IsValidRoom(x, y)) return;
    map[x][y].exists = 1;
    roomCount++;
}
void LevelManager::ChangeRoom(glm::ivec2 direction){//eswn

    currentRoom += direction;

    map[currentRoom.x][currentRoom.y].isVisible = true;
    
    isOpenCurrentDoor = false;

    m_Tilemap->SetDoors(map[currentRoom.x][currentRoom.y].doors[0],
        map[currentRoom.x][currentRoom.y].doors[1],
        map[currentRoom.x][currentRoom.y].doors[2],
        map[currentRoom.x][currentRoom.y].doors[3]);
    //===============================================Map===================================================
    std::vector<UI::Room> RoomForMap;
    std::cout << "===================================Push Romm======================================" << std::endl;
    for (int y = -1; y < 2; y++) {
        for (int x = -1; x < 2; x++) {
            if (x + currentRoom.x < 0 || x + currentRoom.x >= MAP_SIZE_WIDTH || y + currentRoom.y < 0 || y + currentRoom.y >= MAP_SIZE_HEIGHT) {
                UI::Room EmptyRoom;
                EmptyRoom.exists = false;
                RoomForMap.push_back(EmptyRoom);
            }
            else {
                RoomForMap.push_back(map[x + currentRoom.x][y + currentRoom.y]);
            }
            std::cout << x + currentRoom.x << "," << y + currentRoom.y<<" ";
        }
    }
    std::cout << "\n";
    std::cout << "====================================Room Data=====================================" << std::endl;
    std::cout << "currentRoom"<<currentRoom.x <<" "<< currentRoom.y << std::endl;
    for (int j = 2; j > -1; j--) {
        for (int i = 0; i < 3; i++) {
            if (!RoomForMap[i+j*3].exists) std::cout << "x ";
            else std::cout << RoomForMap[i + j * 3].roomType << " ";
        }
        std::cout << "\n";
    }
    m_UI->SetMap(RoomForMap);

    for (auto& obj : currentObjects) {
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
        std::shared_ptr<Table> table = std::dynamic_pointer_cast<Table>(obj);
        std::shared_ptr<Trap> trap = std::dynamic_pointer_cast<Trap>(obj);
        std::shared_ptr<Elevator> elevator = std::dynamic_pointer_cast<Elevator>(obj);
        std::shared_ptr<Actor> actor = std::dynamic_pointer_cast<Actor>(obj);
        std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(obj);
        if (enemy) {
            if (enemy->m_IRangedAttack) {
                enemy->m_IRangedAttack->m_BulletBox->ChangeRoom();
                std::cout << "enemy bullet\n";
            }
            enemy->SetActive(false);
        }
        else if(table){
            table->m_collider->isActive = false;
        }
        else if (trap) {
            trap->isOpen = false;
        }
        else if (elevator) {
            elevator->SetActive(false);
        }
        else if (chest) {
            chest->SetActive(false);
        }
        else if(actor){
            actor->m_collider->isActive = false;
        }
        this->RemoveChild(obj);
    }
    currentObjects.clear();
    for (auto& obj : map[currentRoom.x][currentRoom.y].roomobjs) {
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
        std::shared_ptr<Table> table = std::dynamic_pointer_cast<Table>(obj);
        std::shared_ptr<Trap> trap = std::dynamic_pointer_cast<Trap>(obj);
        std::shared_ptr<Elevator> elevator = std::dynamic_pointer_cast<Elevator>(obj);
        std::shared_ptr<Actor> actor = std::dynamic_pointer_cast<Actor>(obj);
        std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(obj);
        if (enemy) {
            enemy->Start();
            enemy->SetPlayer(m_Player);
            if(!enemy->isDead) enemy->SetActive(true);
        }
        else if (table && !table->isBroken) {
            table->m_collider->isActive = true;
        }
        else if (trap) {
            trap->Start();
            trap->isOpen = true;
        }
        else if (elevator) {
            if(floor!=1 || elevator->isBossRoom) elevator->SetActive(true);
        }
        else if (actor) {
            actor->m_collider->isActive = true;
        }
        else if (chest) {
            std::cout << "chest set\n";
            chest->SetActive(true);
        }
        currentObjects.push_back(obj);
        this->AddChild(obj);
    }
    for (auto& item : map[currentRoom.x][currentRoom.y].roomItems) {
        item->m_collider->isActive = true;
        currentObjects.push_back(item);
        this->AddChild(item);
    }
}
void LevelManager::GenerateLevel() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist_x(0, MAP_SIZE_WIDTH - 1);
    std::uniform_int_distribution<int> dist_y(0, MAP_SIZE_HEIGHT - 1);

    int attempts = 0;
    const int MAX_ATTEMPTS = 1000;

    do {
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
            for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
                map[x][y].exists = 0;
                std::fill(std::begin(map[x][y].doors), std::end(map[x][y].doors), false);
            }
        }
        roomCount = 0;

        startPos = { dist_x(rng), dist_y(rng) };
        AddRoom(startPos.x, startPos.y);
        map[startPos.x][startPos.y].roomType = Tilemap::StartRoom;
        map[startPos.x][startPos.y].isVisible = true;
        currentRoom = startPos;

        std::queue<glm::ivec2> roomQueue;
        roomQueue.push(startPos);

        while (roomCount < MAX_ROOMS) {
            if (roomQueue.empty()) break;
            glm::ivec2 current = roomQueue.front();
            roomQueue.pop();

            std::vector<glm::ivec2> directions = { {1, 0}, {-1, 0}, {0, -1}, {0, 1} };
            std::shuffle(directions.begin(), directions.end(), rng);

            for (auto& dir : directions) {
                int nx = current.x + dir.x;
                int ny = current.y + dir.y;

                if (roomCount >= MAX_ROOMS) break;
                if (IsValidRoom(nx, ny)) {
                    AddRoom(nx, ny);
                    roomQueue.push({ nx, ny });

                    if (dir.x == 1) { 
                        map[current.x][current.y].doors[0] = true; 
                        map[nx][ny].doors[2] = true; 
                        map[nx][ny].entrance = 2;
                    }
                    else if (dir.x == -1) { 
                        map[current.x][current.y].doors[2] = true; 
                        map[nx][ny].doors[0] = true;
                        map[nx][ny].entrance = 0;
                    }
                    else if (dir.y == 1) { 
                        map[current.x][current.y].doors[3] = true;
                        map[nx][ny].doors[1] = true;
                        map[nx][ny].entrance = 1;
                    }
                    else if (dir.y == -1) { 
                        map[current.x][current.y].doors[1] = true; 
                        map[nx][ny].doors[3] = true;
                        map[nx][ny].entrance = 3;
                    }
                }
            }
        }

        attempts++;
        if (attempts >= MAX_ATTEMPTS) {
            std::cerr << "Error: Failed to generate a valid level after " << MAX_ATTEMPTS << " attempts.\n";
            return;
        }

    } while (roomCount < MIN_ROOMS);

    int maxDistance = 0;
    bossPos = { -1, -1 };
    for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
        for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
            if (map[x][y].exists) {
                int dist = abs(x - startPos.x) + abs(y - startPos.y);
                if (dist > maxDistance) {
                    maxDistance = dist;
                    bossPos = { x, y };
                }
            }
        }
    }
    map[bossPos.x][bossPos.y].roomType = Tilemap::BossRoom;

    std::vector<glm::ivec2> availableRooms;
    for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
        for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
            if (map[x][y].exists && glm::ivec2(x, y) != startPos && glm::ivec2(x, y) != bossPos) {
                availableRooms.push_back({ x, y });
            }
        }
    }
    std::shuffle(availableRooms.begin(), availableRooms.end(), rng);

    if (availableRooms.size() >= 2) {
        map[availableRooms[0].x][availableRooms[0].y].roomType = Tilemap::RoomType::shop;
        map[availableRooms[1].x][availableRooms[1].y].roomType = Tilemap::RoomType::treasureRoom;
        availableRooms.erase(availableRooms.begin(), availableRooms.begin() + 2);
    }

    int half = availableRooms.size() / 2;
    std::set<Tilemap::RoomType> usedRoomTypes;
    std::uniform_int_distribution<int> dist_easy(m_Tilemap->seRoom[2], m_Tilemap->seRoom[3]);
    std::uniform_int_distribution<int> dist_medium(m_Tilemap->seRoom[4], m_Tilemap->seRoom[5]);
    std::uniform_int_distribution<int> dist_hard(m_Tilemap->seRoom[6], m_Tilemap->seRoom[7]);

    for (size_t i = 0; i < availableRooms.size(); i++) {
        Tilemap::RoomType roomType;
        do {
            if(floor == 1){
                if (i < half+1) {
                    roomType = static_cast<Tilemap::RoomType>(dist_easy(rng));
                }
                else {
                    roomType = static_cast<Tilemap::RoomType>(dist_medium(rng));
                }
            }
            else if (floor == 2) {
                if (i < half) {
                    roomType = static_cast<Tilemap::RoomType>(dist_medium(rng));
                }
                else {
                    roomType = static_cast<Tilemap::RoomType>(dist_easy(rng));
                }
            }
            else{
                if (i < half) {
                    roomType = static_cast<Tilemap::RoomType>(dist_hard(rng));
                }
                else {
                    roomType = static_cast<Tilemap::RoomType>(dist_medium(rng));
                }
            }
        } while (usedRoomTypes.find(roomType) != usedRoomTypes.end());

        usedRoomTypes.insert(roomType);
        map[availableRooms[i].x][availableRooms[i].y].roomType = roomType;
    }
    m_Tilemap->SetDoors(map[startPos.x][startPos.y].doors[0], map[startPos.x][startPos.y].doors[1], map[startPos.x][startPos.y].doors[2], map[startPos.x][startPos.y].doors[3]);
    std::vector<UI::Room> RoomForMap;
    for(int y=-1;y<2;y++){
        for (int x = -1; x < 2; x++) {
            if (x + startPos.x < 0 || x + startPos.x >= MAP_SIZE_WIDTH || y + startPos.y < 0 || y + startPos.y >= MAP_SIZE_HEIGHT) {
                UI::Room EmptyRoom;
                EmptyRoom.exists = false;
                RoomForMap.push_back(EmptyRoom);
            }
            else {
                RoomForMap.push_back(map[x + startPos.x][y + startPos.y]);
            }
        }
    }
    for (int x = 0; x < MAP_SIZE_WIDTH; x++) {
        for (int y = 0; y < MAP_SIZE_HEIGHT; y++) {
            std::vector<std::shared_ptr<Object>> temp = m_Tilemap->InitRoom((Tilemap::RoomType)map[x][y].roomType, map[x][y].entrance,floor==1?4: (floor == 2 ? 6:8 ));
            if (!temp.empty()) {
                for (auto& obj : temp) {
                    std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj);
                    std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(obj);
                    if (enemy) {
                        enemy->SetPlayer(m_Player);
                        map[x][y].roomobjs.push_back(obj);
                    }
                    else if (item) {
                        map[x][y].roomItems.push_back(item);
                    }
                    else {
                        map[x][y].roomobjs.push_back(obj);
                    }
                }
            }
        }
    }
    std::cout << "RoomForMap:";
    for (auto& room : RoomForMap) {
        std::cout << room.exists;
    }
    std::cout << std::endl;
    m_UI->SetMap(RoomForMap);
    PrintMap();
    if (floor != 1) {
        for (auto& obj : map[currentRoom.x][currentRoom.y].roomobjs) {
            std::shared_ptr<Elevator> elevator = std::dynamic_pointer_cast<Elevator>(obj);
            if (elevator) {
                elevator->SetActive(true);
            }
            currentObjects.push_back(obj);
            this->AddChild(obj);
        }
    }
}

void LevelManager::PrintMap() {
    for (int y = MAP_SIZE_HEIGHT - 1; y >= 0; y--) {  
        for (int x = 0; x < MAP_SIZE_WIDTH; x++) {    
            if (map[x][y].exists) {
                if (glm::ivec2(x, y) == startPos) std::cout << "S ";
                else if (glm::ivec2(x, y) == bossPos) std::cout << "B ";
                else if (map[x][y].roomType == Tilemap::RoomType::shop) std::cout << "$ ";
                else if (map[x][y].roomType == Tilemap::RoomType::treasureRoom) std::cout << "T ";
                else std::cout << map[x][y].roomType << " ";
            }
            else {
                std::cout << "0 ";
            }
        }
        std::cout << std::endl;
    }
}

