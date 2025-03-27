#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Core/Actor.hpp"
class Player;
class Item;
class Enemy;
class BulletBox;

class IBoss {
public:
    virtual std::vector<std::shared_ptr<Object>> summon()=0;
    bool isSummon = false;
};
class IRangedAttack: public Object {
public:
    IRangedAttack(std::weak_ptr<Enemy> ownerEnemy, std::weak_ptr<Util::Animation> m_AnimationShoot,int ammoIndex);
    std::weak_ptr<Enemy> ownerEnemy;
    std::weak_ptr<Util::Animation> m_AnimationShoot;
    float BulletDamage = 1;

    virtual void Shoot();
    std::shared_ptr<BulletBox> m_BulletBox=std::make_shared<BulletBox>();
    bool isAnimDone = false;
    float shootSpeed = 5;
    int ammoIndex=1;
    float m_LastShootTime = 0.0f;
    bool isFire = false;
    float currentTime = 0;

protected:
};
class IMeleeTrigger : public Actor,public Trigger {
public:
    std::weak_ptr<Enemy> ownerEnemy;
    float MeleeDamage = 1;
    IMeleeTrigger(glm::vec2 size);

    void FlipTrigger();

    void OnTriggerStay(std::shared_ptr<BoxCollider>  other) override;
};
class Enemy : public Actor {
public:
    virtual void Start();
    virtual void Update()=0;
    Enemy(glm::vec2 size);
    virtual void SetPlayer(std::weak_ptr<Player> _player);//child obj need add app
    glm::vec2 normalize(glm::vec2 values);

    virtual void SetActive(bool isActive);
    void SetHealth(float _health) { health = _health; }
    inline float GetHealth() { return health; }
    void Damage(float damage) { SetHealth(GetHealth() - damage); }
    bool isDropCoin = false;
    bool isGiveCoin = false;
    std::shared_ptr<Item> GetCoin();
    inline bool GetIsFacingRight() { return isFaceRight; }


    std::shared_ptr<IMeleeTrigger> m_meleeTrigger;
    std::shared_ptr<IRangedAttack> m_IRangedAttack;
    std::weak_ptr<Player> m_Player;

    enum State
    {
        Walk,
        Attack,
        Die,
    };
    State state = Walk;
    std::string m_throwSFX = RESOURCE_DIR "/sound/throw.mp3";
protected:
    std::shared_ptr<Util::Animation> m_AnimationWalk;
    std::shared_ptr<Util::Animation> m_AnimationAttack;
    std::shared_ptr<Util::Animation> m_AnimationDie;
    void PlayDeadSE(std::string &path) {
        std::cout << "Dead\n";
        m_SFX->LoadMedia(path);
        m_SFX->Play();
    }

    void FlipControl();

    bool isFaceRight = true;
    bool isMoving = false;
    float speed = 5;

    float health = 10;
    float maxHealth;
    float damage = 1;

    std::string dead_SEPath = RESOURCE_DIR "/sound/dead_bozz.mp3";
};

#endif
