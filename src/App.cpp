#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

void App::Start() {
    LOG_TRACE("Start");

    //Layer
    std::vector<std::shared_ptr<Util::GameObject>> BackgroundLayer = {m_Tilemap };
    std::vector<std::shared_ptr<Util::GameObject>> PlayerLayer = { m_Player, m_Hand };

    int i = 0;
    for each (auto & obj in BackgroundLayer) {
        obj->SetZIndex(i);
        m_Root.AddChild(obj);
        i++;
    }
    for each (auto& obj in PlayerLayer) {
        obj->SetZIndex(i);
        m_Root.AddChild(obj);
        i++;
    }

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    
    m_Player->Update();
    m_Hand->Update();
    m_Root.Update();
    
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
