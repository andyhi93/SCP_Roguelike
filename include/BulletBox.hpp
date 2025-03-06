#ifndef BULLETBOX_HPP
#define BULLETBOX_HPP

#include "pch.hpp"

#include "Util/GameObject.hpp"
#include "Bullet.hpp"
#include "Util/Image.hpp"

class BulletBox : public Util::GameObject {
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
