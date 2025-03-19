#ifndef SCP743B_HPP
#define SCP743B_HPP

#include "Enemy.hpp"
class SCP743B : public Enemy{
public:
    SCP743B();
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void SetActive(bool isActive) override;

    void Behavior();
    void SetPlayer(std::weak_ptr<Player> _player) override;
protected:
private:
};

#endif
