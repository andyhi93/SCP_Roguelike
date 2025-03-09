#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Core/Solid.hpp"
#include <iostream>

void App::Start() {
    LOG_TRACE("Start");

    //Layer
    std::shared_ptr<Object> empty;
    std::vector<std::shared_ptr<Object>> Objects = { m_LevelManager, empty/*Enemy*/, m_Player};

    int i = 0;
    for each (auto & obj in Objects) {
        if (obj) {
            obj->SetZIndex(i);
            m_Root.AddChild(obj);
        }
        else { LOG_INFO("Error: Null GameObject:{}",i); }
        i++;
    }

    Solid::InitializeColliders();

    m_Player->Start();
    m_LevelManager->setPlayer(m_Player);
    m_Player->SetLevelManager(m_LevelManager);

    m_LevelManager->m_MapUI->GetPlayer(m_Player);

    m_CurrentState = State::UPDATE;
}
void App::FixedUpdate() {
    m_Player->FixedUpdate();
    m_LevelManager->FixedUpdate();
}
float App::GetDeltaTime() {
    static Uint32 lastTime = SDL_GetTicks(); 
    Uint32 currentTime = SDL_GetTicks(); 
    float deltaTime = (currentTime - lastTime) / 1000.0f; 
    lastTime = currentTime; 
    return deltaTime;
}
void App::Update() {
    ColliderManager::GetInstance().Update();
    m_Player->Update();
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
