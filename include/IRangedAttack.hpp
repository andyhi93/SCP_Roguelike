#ifndef IRANGEDATTACK_HPP
#define IRANGEDATTACK_HPP

#include "Bullet.hpp"
#include "BulletBox.hpp"
#include <memory>

class IRangedAttack {
public:
    virtual ~IRangedAttack() = default;

    // ���{������k�A���ĤH�i�H�o�g�l�u
    virtual void Shoot() = 0;

protected:
    bool isFire = false;
    std::shared_ptr<BulletBox> m_BulletBox;
};

#endif
