#ifndef ITEM_HPP
#define ITEM_HPP

#include "pch.hpp"
#include "Core/Actor.hpp"
#include "UIText.hpp"

class Item : public Actor,public Trigger {
public:
    enum ItemType {
        bloodCoin,//0
        bloodPill,
        SCP1609,
        SCP610,
        SCP023,
        SCP1016,//5
        SCP682,
        SCP553,
        SCP668,
        SCP307,
        SCP063,//10
        SCP2295,
    };
    Item(glm::vec2 pos, ItemType type);
    void SetItemType(ItemType type);
    ItemType GetItemType() { return ItemIndex; }
    std::shared_ptr<Item> SetSell();

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

    bool isUnlocked = true;
    static int lastItemIndex;
private:
    ItemType ItemIndex;
    std::shared_ptr<UIText> m_priceText;
    std::shared_ptr<Object> backgroundImage;
};

#endif
