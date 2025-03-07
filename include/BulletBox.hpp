#ifndef BULLETBOX_HPP
#define BULLETBOX_HPP

#include "pch.hpp"

#include "Bullet.hpp"
#include "Util/Image.hpp"
#include "Charater.hpp"

class BulletBox : public Charater{
public:
    BulletBox();

    void Update();
    void AutoRemove();
    void AddBullet(std::shared_ptr<Bullet> bullet);
    void RemoveAll();
    void setLevelManager(std::shared_ptr<LevelManager> _LevelManager);
    void setPlayer(std::shared_ptr<Player> _Player);
    std::shared_ptr<LevelManager> m_LevelManager;
    std::shared_ptr<Player> m_Player;
private:
    std::vector<std::shared_ptr<Bullet>> bullets;
};

#endif
