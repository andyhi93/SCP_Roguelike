#ifndef SCP3199_HPP
#define SCP3199_HPP

#include "Enemy.hpp"
class SCP3199 : public Enemy {
public:
    SCP3199();
    void OnCollisionEnter(std::shared_ptr<BoxCollider> other);
    void Start() override;
    void Update() override;

    void Behavior();
    void valueMul(float rate) override {
        health *= rate;
        adultHealth = health;
        childHealth = adultHealth / 2 + 2;
        damage *= rate;
    }
protected:
private:
    bool isFire = false;

    std::shared_ptr<Util::Animation> m_AnimationEggIdle= std::make_shared<Util::Animation>(
        std::vector<std::string>{RESOURCE_DIR "/SCP3199/egg_idle1.png", RESOURCE_DIR "/SCP3199/egg_idle2.png", }, true, 500, true, 500);
    std::shared_ptr<Util::Animation> m_AnimationEggBorn= std::make_shared<Util::Animation>(
        std::vector<std::string>{RESOURCE_DIR "/SCP3199/egg_crack1.png", RESOURCE_DIR "/SCP3199/egg_crack2.png",
        RESOURCE_DIR "/SCP3199/egg_crack3.png", RESOURCE_DIR "/SCP3199/egg_crack4.png", }, true, 500, false, 500);
    std::shared_ptr<Util::Animation> m_AnimationChildWalk= std::make_shared<Util::Animation>(
        std::vector<std::string>{RESOURCE_DIR "/SCP3199/SCP3199_2_walk1.png", RESOURCE_DIR "/SCP3199/SCP3199_2_walk2.png", }, true, 100, true, 100);
    std::shared_ptr<Util::Animation> m_AnimationChildDie = std::make_shared<Util::Animation>(
        std::vector<std::string>{  RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 50, false, 50);
    enum scp3199State {
        walk,
        die,
        eggIdle,
        eggBorn,
        childWalk,
        childDie,
    };
    scp3199State currentState = walk;

    float adultHealth;
    float childHealth;
    float currentTime = 0;
    float startSpawnTime = 0.0f;
    float startGrowTime = 0.0f;
};

#endif
