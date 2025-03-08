#ifndef SCP743A_HPP
#define SCP743A_HPP

#include "Enemy.hpp"
class SCP743A : public Enemy {
public:
    SCP743A();
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);
    void Update() override;

    void Behavior();
protected:
private:
    bool isFire = false;
};

#endif
