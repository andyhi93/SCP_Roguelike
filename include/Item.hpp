#ifndef ITEM_HPP
#define ITEM_HPP

#include "pch.hpp"
#include "Core/Actor.hpp"

class Item : public Actor {
public:
    enum ItemType {
        redPill,
        redCoin,
        SCP1609,
        SCP610,
        SCP023,
        SCP1016,
        SCP297,
        SCP682,
        SCP553,
        SCP668,
        SCP307,
        SCP063,
        SCP2295,
    };
    Item(glm::vec2 pos, ItemType type);
    void SetItemType(ItemType type);

    void Update() override;
    void pickUp();
    void Start();
    float attackSpeed = 1;
    float movekSpeed = 1;
    float damageUp = 1;
    float healthUp = 1;
    float dashCooldown = 1;
    int healValue = 0;
    bool canPick;
    bool isPick = false;
private:
    ItemType ItemIndex;
};

#endif
