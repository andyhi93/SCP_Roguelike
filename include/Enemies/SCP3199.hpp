#ifndef SCP3199_HPP
#define SCP3199_HPP

#include "Enemy.hpp"
class SCP3199 : public Enemy {
public:
    SCP3199();
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);
    void Start() override;
    void Update() override;
    std::vector<std::shared_ptr<Object>> summon();
    bool isSummonDone = false;
    bool isSummon = false;

    void Behavior();
protected:
private:
    bool isFire = false;
};

#endif
