#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "BulletBox.hpp"
#include "Hand.hpp"
#include "LevelManager.hpp"
#include "Core/Actor.hpp"

class Player : public Actor, public Trigger{
public:
    std::shared_ptr<Hand> m_Hand;
    std::shared_ptr<BulletBox> m_BulletBox;
    std::shared_ptr<LevelManager> m_LevelManager;

    void SetLevelManager(std::shared_ptr<LevelManager> _LevelManager) { m_LevelManager = _LevelManager; }
    
    Player();
    void PlayerControl();
    void AnimationControl();
    void HandControl();
    void Move(glm::vec2& velocity);

    void OnTriggerEnter(std::shared_ptr<BoxCollider> other) override;
    void OnTriggerStay(std::shared_ptr<BoxCollider> other) override;

    void Start();
    void Update() override;
    void FixedUpdate() override;
    void Damage(float damage);
    void SetHealth(float amount);
    float GetHealth();
    float GetCurrentHealth() { return currentHealth; }
    bool GetIsInvincible() { return isInvincible; }

    void SetCoin(int amount) { coinAmount = amount; }
    int GetCoin() { return coinAmount; }

    bool isDashing = false;

    bool getCanDash() { return canDash; }
protected:
    std::shared_ptr<Util::Animation> m_AnimationIdle;
    std::shared_ptr<Util::Animation> m_AnimationWalk;
    std::shared_ptr<Util::Animation> m_AnimationDash;
    std::shared_ptr<Util::Animation> m_AnimationHurt;
    std::shared_ptr<Util::Animation> m_AnimationDie;
private:
    enum State
    {
        Idle,
        Walk,
        Hurt,
        Die,
    };
    State currentState = Idle;
    bool isFaceRight = true;
    //bool isMoving = false;
    float speed = 5;
    float m_LastShotTime=0;
    float m_ShotInterval=1;
    float ammoDamage = 1;
    float maxHealth = 5;
    float currentHealth = maxHealth;
    int coinAmount = 100;

    float dashTime = 0.2f;  
    float dashCooldown = 3.0f;  
    float dashSpeedMultiplier = 5.0f;  
    float dashStartTime = 0.0f;  
    float lastDashEndTime = -dashCooldown;  
    bool canDash = true;  

    float invincibleTime = 1.5f;
    bool isInvincible = false;
    float lastHurtTime = 0.0f;
};

#endif
