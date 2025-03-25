#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Core/Solid.hpp"
#include <iostream>

void App::Start() {
    LOG_TRACE("Start");
    m_Menu = std::make_shared<Menu>();


    isSetMenu = true;
    m_Menu->OpenMenu();
    m_Root.AddChild(m_Menu);

    m_CurrentState = State::UPDATE;
}
void App::InitMobMap() {
    m_Player->m_Transform.translation = { 0,0 };
    //Layer
    m_LevelManager = std::make_shared<LevelManager>(true,floor);
    std::shared_ptr<Object> empty;
    std::vector<std::shared_ptr<Object>> Objects = { m_LevelManager, empty/*Enemy*/, m_Player };

    int i = 0;
    for each (auto & obj in Objects) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}", i); }
        i++;
    }

    //Solid::InitializeColliders();

    m_Player->Start();
    m_LevelManager->setPlayer(m_Player);
    m_Player->SetLevelManager(m_LevelManager);

    m_LevelManager->m_UI->SetPlayer(m_Player);
}
void App::FreeMobMap() {
    m_Root.RemoveChild(m_Player);
    m_Root.RemoveChild(m_LevelManager);

    m_LevelManager.reset();
    ColliderManager::GetInstance().ClearCollider();
    ColliderManager::GetInstance().RegisterCollider(m_Player->m_collider);
}
void App::ResetGame() {
    FreeMobMap();
    m_Player.reset();
    floor = 1;
}
void App::InitBossMap() {
    m_Player->m_Transform.translation = { 0,0 };
    m_Player->m_WorldCoord = { 0,0 };
    m_LevelManager = std::make_shared<LevelManager>(false,2);
    m_LevelManager->setPlayer(m_Player);
    if (floor == 1 || floor == 3) m_LevelManager->isSCP049 = true;
    else m_LevelManager->isSCP049 = false;
    m_LevelManager->InitBossRoom();
    std::shared_ptr<Object> empty;
    std::vector<std::shared_ptr<Object>> Objects = { m_LevelManager, empty/*Enemy*/, m_Player };

    int i = 0;
    for each (auto & obj in Objects) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}", i); }
        i++;
    }
    m_LevelManager->setPlayer(m_Player);
    m_Player->SetLevelManager(m_LevelManager);
}
void App::FreeBossMap() {
    ++floor;
    m_Root.RemoveChild(m_Player);
    m_Root.RemoveChild(m_LevelManager);

    m_LevelManager.reset();
    ColliderManager::GetInstance().ClearCollider();
    ColliderManager::GetInstance().RegisterCollider(m_Player->m_collider);
}
void App::FixedUpdate() {
    if (isInitMap && !isStop) {
        m_Player->FixedUpdate();
        m_LevelManager->FixedUpdate();
    }
}
float App::GetDeltaTime() {
    static Uint32 lastTime = SDL_GetTicks(); 
    Uint32 currentTime = SDL_GetTicks(); 
    float deltaTime = (currentTime - lastTime) / 1000.0f; 
    lastTime = currentTime; 
    return deltaTime;
}
void App::Update() {
    m_Root.Update();
    //std::cout << "Ref count: " << m_Player.use_count() << std::endl;
    if (isInitMap && !isStop) {
        ColliderManager::GetInstance().Update();
        m_Player->Update();
        m_LevelManager->Update();
    }

    m_Menu->Update();
    if (isSetMenu) { 
        if (currentGameState == StartMenu) {
            if (m_Menu->startButton->isClick()) {
                m_Player = std::make_shared<Player>();
                m_Menu->CloseMenu();
                m_Root.RemoveChild(m_Menu);
                currentGameState = MobFloor;
                m_Menu->isStartMenu = false;
                isSetMenu = false;
                isInitMap = true;
                InitMobMap();
            }
            if (m_Menu->exitButton->isClick()) {
                m_CurrentState = State::END;
            }
        }
        else {
            if (m_Menu->resumeButton->isClick()) {
                isSetMenu = false;
                m_Menu->CloseMenu();
                m_Root.RemoveChild(m_Menu);
                isStop = false;
            }
            if (m_Menu->menuButton->isClick()&& isInitMap) {
                currentGameState = StartMenu;
                m_Menu->isStartMenu = true;
                m_Menu->OpenMenu();
                isInitMap = false;
                isStop = false;
                FreeMobMap();
            }
        }
    }
    else {
        if (currentGameState != StartMenu /* && !isSetMenu */ && Util::Input::IsKeyDown(Util::Keycode::ESCAPE)) {
            isSetMenu = true;
            m_Menu->OpenMenu();
            m_Root.AddChild(m_Menu);
            isStop = true;
        }
    }
    if (Util::Input::IsKeyUp(Util::Keycode::I) && !isInitMap) {
        isInitMap = true;
        InitMobMap();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::O) && isInitMap) {
        isInitMap = false;
        FreeMobMap();
    }
    if (!currentGameState==StartMenu && (m_Player->isElevate || Util::Input::IsKeyUp(Util::Keycode::B) && isInitMap)) {
        if (currentGameState == MobFloor) {
            m_Player->isElevate = false;
            currentGameState = Boss;
            FreeMobMap();
            InitBossMap();
        }
        else if (currentGameState == Boss) {
            m_Player->isElevate = false;
            currentGameState = MobFloor;
            FreeBossMap();
            InitMobMap();
        }
    }
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    /*if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }*/
    //FixedUpdate
    float deltaTime = GetDeltaTime(); 
    m_AccumulatedTime += deltaTime;

    while (m_AccumulatedTime >= m_FixedDeltaTime) {
        FixedUpdate(); 
        m_AccumulatedTime -= m_FixedDeltaTime; 
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
