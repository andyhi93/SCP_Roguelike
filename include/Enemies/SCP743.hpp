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

    std::shared_ptr<Object> summon();
    bool isSummon = false;
protected:
private:
    std::shared_ptr<Util::Animation> m_AnimationIdle;
    enum AttackState {
        StopShoot,
        Shoot,
        Summon,
    };
    AttackState currentAttackState = StopShoot;
    int shootIndex = 0;
    float summonColdDown = 5;
    float m_LastSummonTime = 0;
    float shootColdDown = 5;
    float m_LastAttackTime = 0;
    float m_FirstAttackTIme = 0;
    float m_attackTime = 5;
    std::vector<int> HealthQuarter = { 0,0,0 };//0:flase 1:moving 2:done

    std::shared_ptr<Object> HealthBackground = std::make_shared<Object>();
    std::shared_ptr<Object> HealthBarImage = std::make_shared<Object>();
    std::shared_ptr<Object> HealthNameImage = std::make_shared<Object>();
};

#endif
