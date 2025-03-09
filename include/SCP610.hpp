#ifndef SCP610_HPP
#define SCP610_HPP

#include "Enemy.hpp"
#include "IRangedAttack.hpp"
class SCP610 : public Enemy ,public IRangedAttack{
public:
    SCP610();
    void Update() override;
    void FixedUpdate() override;

    void Behavior();
    void Shoot() override;
    void SetPlayer(std::shared_ptr<Player> _player) override;
protected:
private:
};

#endif
