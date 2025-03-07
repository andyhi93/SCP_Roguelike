#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Collider.hpp"
#include "Charater.hpp"
class Player;
class Enemy : public Charater{
public:
    virtual void Update()=0;
    Enemy();
    virtual void SetPlayer(std::shared_ptr<Player> _player);//child obj need add app
    glm::vec2 normalize(glm::vec2 values);

    std::shared_ptr<Collider> m_Collider;

    void SetHealth(float _health);
    float GetHealth() { return health; }
    bool isDead = false;
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
    float attackSpeed = 5;
    float m_LastAttackTime = 0.0f;   

    float health = 10;
    float damage = 1;
};

#endif
