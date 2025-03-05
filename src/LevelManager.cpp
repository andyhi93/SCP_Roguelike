#include "LevelManager.hpp"
#include <queue>
#include <random>
#include <iostream>
#include "Player.hpp"

LevelManager::LevelManager() {
    m_MapUI= std::make_shared<MapUI>();
    this->AddChild(m_Tilemap);
    this->AddChild(m_MapUI);
    m_MapUI->SetZIndex(10);
    GenerateLevel();
}
void LevelManager::setPlayer(std::shared_ptr<Player> _player) {
    m_Player = _player;
    std::vector<std::shared_ptr<Enemy>> temp = m_Tilemap->InitRoom(Tilemap::Room610);
    currentEnemies.clear();
    for (auto& obj : temp) {
        obj->GetPlayer(m_Player);
        this->AddChild(obj);
        currentEnemies.push_back(obj);
    }
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
    m_MapUI->Update();
    for (auto& obj : currentEnemies) {
        obj->Update();
    }
}

void LevelManager::AddRoom(int x, int y) {
    if (!IsValidRoom(x, y)) return;
    map[x][y].exists = 1;
    roomCount++;
}
void LevelManager::ChangeRoom(glm::ivec2 direction){//eswn
    currentRoom += direction;
    m_Tilemap->SetDoors(map[currentRoom.x][currentRoom.y].doors[0],
        map[currentRoom.x][currentRoom.y].doors[1],
        map[currentRoom.x][currentRoom.y].doors[2],
        map[currentRoom.x][currentRoom.y].doors[3]);
    std::vector<MapUI::Room> RoomForMap;
    std::cout << "===================================Push Romm======================================" << std::endl;
    for (int y = -1; y < 2; y++) {
        for (int x = -1; x < 2; x++) {
            if (x + currentRoom.x < 0 || x + currentRoom.x >= MAP_SIZE_WIDTH || y + currentRoom.y < 0 || y + currentRoom.y >= MAP_SIZE_HEIGHT) {
                MapUI::Room EmptyRoom;
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
    m_MapUI->SetMap(RoomForMap);
}
void LevelManager::GenerateLevel() {
    floor++;
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

                    if (dir.x == 1) { map[current.x][current.y].doors[0] = true; map[nx][ny].doors[2] = true; }
                    else if (dir.x == -1) { map[current.x][current.y].doors[2] = true; map[nx][ny].doors[0] = true; }
                    else if (dir.y == 1) { map[current.x][current.y].doors[3] = true; map[nx][ny].doors[1] = true; }
                    else if (dir.y == -1) { map[current.x][current.y].doors[1] = true; map[nx][ny].doors[3] = true; }
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
    std::uniform_int_distribution<int> dist_easy(Tilemap::RoomType::Room610,Tilemap::RoomType::Room049_2);
    std::uniform_int_distribution<int> dist_medium(Tilemap::RoomType::Room1048B_1048C,Tilemap::RoomType::Room3119);
    std::uniform_int_distribution<int> dist_hard(Tilemap::RoomType::Room1048_610, Tilemap::RoomType::Room553_610_743ant);

    for (size_t i = 0; i < availableRooms.size(); i++) {
        Tilemap::RoomType roomType;
        do {
            if(floor == 1){
                if (i < half) {
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
    std::vector<MapUI::Room> RoomForMap;
    for(int y=-1;y<2;y++){
        for (int x = -1; x < 2; x++) {
            if (x + startPos.x < 0 || x + startPos.x >= MAP_SIZE_WIDTH || y + startPos.y < 0 || y + startPos.y >= MAP_SIZE_HEIGHT) {
                MapUI::Room EmptyRoom;
                EmptyRoom.exists = false;
                RoomForMap.push_back(EmptyRoom);
            }
            else {
                RoomForMap.push_back(map[x + startPos.x][y + startPos.y]);
            }
        }
    }
    std::cout << "RoomForMap:";
    for (auto& room : RoomForMap) {
        std::cout << room.exists;
    }
    std::cout << std::endl;
    m_MapUI->SetMap(RoomForMap);
    PrintMap();
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

