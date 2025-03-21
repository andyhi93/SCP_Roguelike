#ifndef SCP553_HPP
#define SCP553_HPP

#include "Enemy.hpp"
class SCP553 : public Enemy{
public:
    SCP553();
    void Update() override;
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);
    void Start() override;

    void Behavior();
    void SetPlayer(std::weak_ptr<Player> _player) override;
protected:
private:
};

#endif
