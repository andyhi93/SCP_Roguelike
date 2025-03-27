#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export

#include "Util/Renderer.hpp"
#include "core/Object.hpp"
#include "Player.hpp"
#include "LevelManager.hpp"
#include "Core/ColliderManager.hpp"
#include "Menu.hpp"
#include "Core/Camera.hpp"
#include <Util/BGM.hpp>

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
    std::shared_ptr<Player> m_Player;
    std::shared_ptr<LevelManager> m_LevelManager;
    std::shared_ptr<Menu> m_Menu;

    void InitMobMap();
    void FreeMobMap();
    void InitBossMap();
    void FreeBossMap();
    void ResetGame();
    bool isInitMap = false;

    Util::Renderer m_Root;

    //FixedUpdate
    float m_FixedDeltaTime = 0.016f; 
    float m_AccumulatedTime = 0.0f;  

    enum GameState {
        StartMenu,
        MobFloor,
        Boss,
    };
    GameState currentGameState = StartMenu;
    bool isSetMenu = false;
    bool isSwitchScene = false;

    bool isStop = false;

    int floor = 1;
    float m_LastDarkTime = 0;

    std::shared_ptr<Util::BGM> m_BGM = std::make_shared<Util::BGM>(RESOURCE_DIR "/sound/title.mp3");
    std::string m_TitleBGMPath = { RESOURCE_DIR "/sound/title.mp3" };
    std::string m_BattleBGMPath = { RESOURCE_DIR "/sound/battle a.mp3" };
    std::string m_SC049aBGMPath = { RESOURCE_DIR "/sound/SC049 a.mp3" };
    std::string m_SC049bBGMPath = { RESOURCE_DIR "/sound/SC049 b.mp3" };
    std::string m_SC743aBGMPath = { RESOURCE_DIR "/sound/SC743 a.mp3" };
    std::string m_SC743bBGMPath = { RESOURCE_DIR "/sound/SC743 b.mp3" };
};

#endif
