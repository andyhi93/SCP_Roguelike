#ifndef SCP3199_2_HPP
#define SCP3199_2_HPP

#include "Enemy.hpp"
class SCP3199_2 : public Enemy {
public:
    SCP3199_2();
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);
    void Start() override;
    void Update() override;

    void Behavior();
protected:
private:
    std::shared_ptr<Util::Animation> m_AnimationIdle;
    std::shared_ptr<Util::Animation> m_AnimationBorn;
    bool isFire = false;
    enum KidState {
        eggIdle,
        eggBorn,
        walk,
        die,
    };
    KidState currentState=eggIdle; 
    
    float currentTime = 0;
    float startSpawnTime = 0.0f;
};

#endif
