#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "BulletBox.hpp"
#include "Hand.hpp"
#include "Collider.hpp"
#include "LevelManager.hpp"

class Player : public Util::GameObject, public std::enable_shared_from_this<Player> {
public:
    std::shared_ptr<Hand> m_Hand;
    std::shared_ptr<BulletBox> m_BulletBox;
    std::shared_ptr<Collider> m_Collider;
    std::shared_ptr<LevelManager> m_LevelManager;
    
    Player();
    void PlayerControl();
    void AnimationControl();
    void HandControl();
    void Move(glm::vec2& velocity);
    void getLevelManager(std::shared_ptr<LevelManager> _LevelManager);

    void Update();

    float health = 10;
protected:
    std::shared_ptr<Util::Animation> m_AnimationIdle;
    std::shared_ptr<Util::Animation> m_AnimationWalk;
    std::shared_ptr<Util::Animation> m_AnimationDash;
    std::shared_ptr<Util::Animation> m_AnimationDie;
private:
    enum State
    {
        Idle,
        Walk,
        Die,
    };
    bool isFaceRight = true;
    bool isMoving = false;
    float speed = 5;
    float m_LastShotTime=0;
    float m_ShotInterval=0.5f;

    float dashTime = 0.2f;  
    float dashCooldown = 3.0f;  
    float dashSpeedMultiplier = 5.0f;  
    float dashStartTime = 0.0f;  
    float lastDashEndTime = -dashCooldown;  
    bool isDashing = false;  
    bool canDash = true;  

};

#endif
