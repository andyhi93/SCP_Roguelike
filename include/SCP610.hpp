#ifndef SCP610_HPP
#define SCP610_HPP

#include "Enemy.hpp"
class SCP610 : public Enemy {
public:
    SCP610();
    void Update() override;

    void Behavior();
protected:
private:
    bool isFire = false;
};

#endif
