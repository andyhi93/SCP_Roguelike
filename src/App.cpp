#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include <iostream>

void App::Start() {
    LOG_TRACE("Start");

    //Layer
    std::shared_ptr<Util::GameObject> empty;
    std::vector<std::shared_ptr<Util::GameObject>> BackgroundLayer = {m_LevelManager , empty };
    std::vector<std::shared_ptr<Util::GameObject>> PlayerLayer = { m_Player, m_Player->m_Hand, m_Player->m_BulletBox };
    std::vector<std::shared_ptr<Util::GameObject>> EnemyLayer = { m_SCP610,m_SCP610->m_BulletBox };
    std::vector<std::shared_ptr<Util::GameObject>> UILayer = { empty};

    m_SCP610->GetPlayer(m_Player);
    m_Player->getLevelManager(m_LevelManager);
    m_LevelManager->m_MapUI->GetPlayer(m_Player);
    m_LevelManager->currentEnemies.push_back(m_SCP610);
    int i = 0;
    for each (auto & obj in BackgroundLayer) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}",i); }
        i++;
    }
    for each (auto& obj in PlayerLayer) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}", i); }
        i++;
    }
    for each (auto & obj in EnemyLayer) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}", i); }
        i++;
    }
    for each (auto & obj in UILayer) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}", i); }
        i++;
    }

    m_CurrentState = State::UPDATE;
}
void App::FixedUpdate() {
    m_Player->m_BulletBox->Update();
    m_SCP610->Update();
}
float App::GetDeltaTime() {
    static Uint32 lastTime = SDL_GetTicks(); 
    Uint32 currentTime = SDL_GetTicks(); 
    float deltaTime = (currentTime - lastTime) / 1000.0f; 
    lastTime = currentTime; 
    return deltaTime;
}
void App::Update() {
    
    m_Player->Update();
    m_Player->m_Hand->Update();
    m_LevelManager->Update();
    m_Root.Update();
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
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
