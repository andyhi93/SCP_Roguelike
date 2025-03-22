#ifndef SCP049_HPP
#define SCP049_HPP

#include "Enemy.hpp"
class SCP049 : public Enemy {
public:
    SCP049();
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void SetActive(bool isActive) override;
    void Shootable();

    void Behavior();
    void SetPlayer(std::weak_ptr<Player> _player) override;
protected:
private:
    std::shared_ptr<Util::Animation> m_AnimationIdle;
    enum State {
        Idle,
        Chase,
        Patrol,
        Summon,
    };
    State currentState=Idle; 
    enum AttackState {
        StopShoot,
        Shoot,
        Shoot2,
    };
    AttackState currentAttackState= Shoot;
    int shootIndex = 0;
};

#endif
