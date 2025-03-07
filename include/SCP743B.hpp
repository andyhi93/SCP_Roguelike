#ifndef SCP743B_HPP
#define SCP743B_HPP

#include "Enemy.hpp"
#include "IRangedAttack.hpp"
class SCP743B : public Enemy, public IRangedAttack {
public:
    SCP743B();
    void Update() override;

    void Behavior();
    void Shoot() override;
    void SetPlayer(std::shared_ptr<Player> _player) override;
protected:
private:
};

#endif
