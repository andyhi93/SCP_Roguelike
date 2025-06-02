#include "App.hpp"

#include "Util/Image.hpp"
#include "core/BoxCollider.hpp"
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

    m_EndImage->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/escape.png"));
    m_EndImage->SetVisible(false);
    m_EndImage->SetZIndex(10);
    m_EndImage->m_Transform.scale = { 4,4 };
    m_Root.AddChild(m_EndImage);

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
    if(currentGameState==MobFloor) FreeMobMap();
    if (currentGameState == Boss) FreeBossMap();
    m_Player.reset();
    floor = 1;
    m_EndImage->SetVisible(false);
}
void App::InitBossMap() {
    m_Player->m_Transform.translation = { 0,0 };
    m_Player->m_WorldCoord = { 0,0 };
    m_LevelManager = std::make_shared<LevelManager>(false,floor);
    m_LevelManager->setPlayer(m_Player);
    if(floor==5) m_LevelManager->bossType = Tilemap::BossType::Both;
    else if (floor % 2 == 1) m_LevelManager->bossType = Tilemap::BossType::RoomSCP049;
    else m_LevelManager->bossType = Tilemap::BossType::RoomSCP743;
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
    /*if (isInitMap && !isStop) {
        m_Player->FixedUpdate();
        m_LevelManager->FixedUpdate();
    }*/
}
float App::GetDeltaTime() {
    static Uint32 lastTime = SDL_GetTicks(); 
    Uint32 currentTime = SDL_GetTicks(); 
    float deltaTime = (currentTime - lastTime) / 1000.0f; 
    lastTime = currentTime; 
    return deltaTime;
}
void App::BGM_AB_Play(std::string Apath, float Alength, std::string Bpath){
    float currentTime = SDL_GetTicks() / 1000.0f;
    if (currentBGMState==Stop) {
        currentBGMState = A;
        m_FirstPlayTime = currentTime;
        m_BGM->LoadMedia(Apath);
        m_BGM->Play();
    }
    else if (currentBGMState == A && currentTime - Alength >= m_FirstPlayTime) {
        currentBGMState = B;
        m_BGM->LoadMedia(Bpath);
        m_BGM->Play();
    }
}
void App::Update() {
    m_Root.Update();
    m_Menu->Update();
    m_BGM->SetVolume(m_Menu->GetSoundPercent()*100); 
    if(m_Player) m_Player->m_SFX->SetVolume(m_Menu->GetSoundPercent() * 100);
    if (m_LevelManager) {
        for (auto& obj : m_LevelManager->currentObjects) {
            if (obj) {
                if (obj->m_SFX) obj->m_SFX->SetVolume(m_Menu->GetSoundPercent() * 100);
            }
        }
    }
    if (Mix_PlayingMusic() == 0) {
        m_BGM->Play();
    }
    //std::cout << "Ref count: " << m_Player.use_count() << std::endl;
    if (floor!=6 && isInitMap && !isStop) {
        ColliderManager::GetInstance().Update();
        m_Player->Update();
        m_LevelManager->Update();
    }
    if (!isSwitchScene && !currentGameState == StartMenu && (m_Player->isElevate || Util::Input::IsKeyUp(Util::Keycode::B) && isInitMap)) {
        isSwitchScene = true;
        m_Player->isElevate = false;
    }
    if (currentGameState == Boss) {
        if (floor%2==1) {
            BGM_AB_Play(m_SC049aBGMPath, 38, m_SC049bBGMPath);
        }
        else {
            BGM_AB_Play(m_SC743aBGMPath, 15, m_SC743bBGMPath);
        }
    }
    else{
        currentBGMState = Stop;
    }
    if (isSwitchScene) {
        float currentTime = SDL_GetTicks() / 1000.0f;
        if (currentGameState == MobFloor) {
            if (!m_Menu->GetIsFading() && !m_Menu->GetIsFullDark()) {
                m_Menu->FadeIn();
                m_LastDarkTime = currentTime;
            }
            if (m_Menu->GetIsFullDark() && currentTime - m_LastDarkTime >= 2) {
                currentGameState = Boss;
                FreeMobMap();
                InitBossMap();
                m_Menu->FadeOut();
                isSwitchScene = false;
            }
        }
        else if (currentGameState == Boss) {
            if (!m_Menu->GetIsFading() && !m_Menu->GetIsFullDark()) {
                m_Menu->FadeIn();
                m_LastDarkTime = currentTime;
            }
            if (m_Menu->GetIsFullDark() && currentTime-m_LastDarkTime>=2) {
                m_BGM->LoadMedia(m_BattleBGMPath);
                m_BGM->Play();
                currentGameState = MobFloor;
                FreeBossMap();
                if(floor==6){
                    m_Menu->FadeOut();
                    m_EndImage->SetVisible(true);
                    isSwitchScene = false;
                }
                else {
                    InitMobMap();
                    m_Menu->FadeOut();
                    isSwitchScene = false;
                }
            }
        }
    }
    if (isSetMenu) { 
        if (currentGameState == StartMenu) {
            if (m_Menu->startButton->isClick()) {
                m_BGM->LoadMedia(m_BattleBGMPath);
                m_BGM->Play();
                m_Player = std::make_shared<Player>();
                m_Menu->CloseMenu();
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
            m_Player->m_SFX->SetVolume(m_Menu->GetSoundPercent() * 100);
            if (m_LevelManager) {
                for (auto& object : m_LevelManager->currentObjects) {
                    if (object->layer != Object::CollisionLayer::Enemy) continue;
                    auto enemy = std::dynamic_pointer_cast<Enemy>(object);
                    if (enemy) enemy->m_SFX->SetVolume(m_Menu->GetSoundPercent() * 100);
                }
            }
            if (m_Menu->resumeButton->isClick()) {
                isSetMenu = false;
                m_Menu->CloseMenu();
                isStop = false;
            }
            if (m_Menu->menuButton->isClick()&& isInitMap) {
                m_BGM->LoadMedia(m_TitleBGMPath);
                m_BGM->Play();
                m_Menu->isStartMenu = true;
                m_Menu->OpenMenu();
                isInitMap = false;
                isStop = false;
                ResetGame();
                currentGameState = StartMenu;
            }
        }
    }
    else {
        if (currentGameState != StartMenu /* && !isSetMenu */ && Util::Input::IsKeyDown(Util::Keycode::ESCAPE)) {
            isSetMenu = true;
            m_Menu->OpenMenu();
            isStop = true;
        }
    }
    if (Util::Input::IsKeyUp(Util::Keycode::V)) {
        BoxCollider::hitboxVisible = !BoxCollider::hitboxVisible;
    }
    if (Util::Input::IsKeyUp(Util::Keycode::H) && m_Player) {
        isCheating = !isCheating;
        m_Player->isCheating = isCheating;
    }
    /*if (Util::Input::IsKeyUp(Util::Keycode::I) && !isInitMap) {
        isInitMap = true;
        InitMobMap();
    }
    if (Util::Input::IsKeyUp(Util::Keycode::O) && isInitMap) {
        isInitMap = false;
        FreeMobMap();
    }*/ 
    
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    /*if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }*/
    //FixedUpdate
    /*float deltaTime = GetDeltaTime();
    m_AccumulatedTime += deltaTime;

    while (m_AccumulatedTime >= m_FixedDeltaTime) {
        FixedUpdate(); 
        m_AccumulatedTime -= m_FixedDeltaTime; 
    }*/
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
