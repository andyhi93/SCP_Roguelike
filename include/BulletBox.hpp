#ifndef BULLETBOX_HPP
#define BULLETBOX_HPP

#include "pch.hpp"

#include "Bullet.hpp"
#include "Util/Image.hpp"

class BulletBox : public Object{
public:
    BulletBox();

    void Update();
    void AutoRemove();
    void AddBullet(std::shared_ptr<Bullet> bullet);
    void RemoveAll();
private:
    std::vector<std::shared_ptr<Bullet>> bullets;
};

#endif
