#ifndef SCP610_HPP
#define SCP610_HPP

#include "Enemy.hpp"
class SCP610 : public Enemy{
public:
    SCP610();
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void SetActive(bool isActive) override;
    void Shootable();

    void Behavior();
    void SetPlayer(std::weak_ptr<Player> _player) override;
protected:
private:
};

#endif
