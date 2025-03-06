#ifndef IRANGEDATTACK_HPP
#define IRANGEDATTACK_HPP

#include "Bullet.hpp"
#include "BulletBox.hpp"
#include <memory>

class IRangedAttack {
public:
    virtual ~IRangedAttack() = default;

    // 遠程攻擊方法，讓敵人可以發射子彈
    virtual void Shoot() = 0;

protected:
    bool isFire = false;
    std::shared_ptr<BulletBox> m_BulletBox;
};

#endif
