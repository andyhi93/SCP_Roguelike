#ifndef SCP1048_B_HPP
#define SCP1048_B_HPP

#include "Enemy.hpp"
#include "random"
class SCP1048_B : public Enemy {
public:
    SCP1048_B();
    void Update() override;
    void Start() override;
    void FixedUpdate() override;
    void SetActive(bool isActive) override;
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);

    void Behavior();
    void SetPlayer(std::weak_ptr<Player> _player) override;
protected:
private:
    std::vector <glm::vec2> patrolPos= { {776, -365},{ -784,-389 }, { -776,338 }, { 776, 336 } };
    int targetIndex=0;
};

#endif
