#ifndef SCP492_HPP
#define SCP0492_HPP

#include "Enemy.hpp"
class SCP049_2 : public Enemy{
public:
    SCP049_2();
    void Update() override;
    void Start() override;

    void Behavior();
protected:
private:
    bool isFire = false;
};

#endif
