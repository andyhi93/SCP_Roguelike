#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Core/Actor.hpp"
class Player;
class Item;
class Enemy;

class IMeleeTrigger : public Actor,public Trigger {
public:
    std::weak_ptr<Enemy> ownerEnemy;
    float MeleeDamage = 1;
    IMeleeTrigger(glm::vec2 size);

    void FlipTrigger();

    void OnTriggerStay(std::shared_ptr<BoxCollider>  other) override;
};
class Enemy : public Actor {
public:
    virtual void Start();
    virtual void Update()=0;
    Enemy(glm::vec2 size);
    virtual void SetPlayer(std::shared_ptr<Player> _player);//child obj need add app
    glm::vec2 normalize(glm::vec2 values);

    virtual void SetActive(bool isActive);
    void SetHealth(float _health) { health = _health; }
    float GetHealth() { return health; }
    void Damage(float damage) { SetHealth(GetHealth() - damage); }
    bool isDropCoin = false;
    bool isGiveCoin = false;
    std::shared_ptr<Item> GetCoin();
    bool GetIsFacingRight() { return isFaceRight; }


    std::shared_ptr<IMeleeTrigger> m_meleeTrigger;
protected:
    std::shared_ptr<Util::Animation> m_AnimationWalk;
    std::shared_ptr<Util::Animation> m_AnimationAttack;
    std::shared_ptr<Util::Animation> m_AnimationDie;

    std::shared_ptr<Player> m_Player;
    void FlipControl();

    enum State
    {
        Walk,
        Attack,
        Die,
    };
    State state = Walk;
    bool isFaceRight = true;
    bool isMoving = false;
    float speed = 5;
    float attackSpeedUp = 5;
    float m_LastAttackTime = 0.0f;   

    float health = 10;
    float damage = 1;
};

#endif
