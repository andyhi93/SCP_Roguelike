#ifndef SCP492_HPP
#define SCP0492_HPP

#include "Enemy.hpp"
class SCP049_2 : public Enemy{
public:
    SCP049_2();
    void Update() override;
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);

    void Behavior();
protected:
private:
    bool isFire = false;
};

#endif
