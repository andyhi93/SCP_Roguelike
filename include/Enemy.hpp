#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"
#include "BulletBox.hpp"
#include "Collider.hpp"
class Player;
class Enemy : public Util::GameObject {
public:
    virtual void Update()=0;
    void GetPlayer(std::shared_ptr<Player> _player);//child obj need add app

protected:
    std::shared_ptr<Util::Animation> m_AnimationWalk;
    std::shared_ptr<Util::Animation> m_AnimationAttack;
    std::shared_ptr<Util::Animation> m_AnimationDie;

    std::shared_ptr<BulletBox> m_BulletBox;
    std::shared_ptr<Collider> m_Collider;
    std::shared_ptr<Player> m_Player = std::make_shared<Player>();
private:
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
    float m_LastShotTime = 0;
    float m_ShotInterval = 0.5f;

    float health = 10;
    float damage = 1;
};

#endif
