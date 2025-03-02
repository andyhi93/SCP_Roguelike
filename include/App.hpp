#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "Player.hpp"
#include "Hand.hpp"
#include "Tilemap.hpp"
#include "BulletBox.hpp"
#include "LevelManager.hpp"
class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();
    void FixedUpdate();
    float GetDeltaTime();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    //std::shared_ptr<Tilemap> m_Tilemap = std::make_shared<Tilemap>();
    std::shared_ptr<Player> m_Player = std::make_shared<Player>();
    std::shared_ptr<BulletBox> m_BulletBox = std::make_shared<BulletBox>();
    std::shared_ptr<LevelManager> m_LevelManager = std::make_shared<LevelManager>();
    Util::Renderer m_Root;

    //FixedUpdate
    float m_FixedDeltaTime = 0.016f; 
    float m_AccumulatedTime = 0.0f;  
};

#endif
