#ifndef SCP1048_C_HPP
#define SCP1048_C_HPP

#include "Enemy.hpp"
#include "IRangedAttack.hpp"
class SCP1048_C : public Enemy, public IRangedAttack {
public:
    SCP1048_C();
    void Update() override;
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);

    void Behavior();
    void Shoot() override;
    void SetPlayer(std::shared_ptr<Player> _player) override;
protected:
private:
};

#endif
