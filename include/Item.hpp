#ifndef ITEM_HPP
#define ITEM_HPP

#include "pch.hpp"
#include "Core/Actor.hpp"

class Item : public Actor,public Trigger {
public:
    enum ItemType {
        bloodPill,
        bloodCoin,
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
    ItemType GetItemType() { return ItemIndex; }

    void Update() override;
    std::vector<float> pickUp();
    void Start();
    float attackSpeedUp = 1;
    float moveSpeedUp = 1;
    float damageUp = 1;
    float healthUp = 1;
    float dashCooldown = 1;
    float healValue = 0;
    float price = 10;
    bool canPick;
    bool isPick = false;
private:
    ItemType ItemIndex;
};

#endif
