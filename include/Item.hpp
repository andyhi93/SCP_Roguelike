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
    std::shared_ptr<Item> SetPickUI();

    void OnTriggerEnter(std::shared_ptr<BoxCollider> other) override;
    void OnTriggerExit(std::shared_ptr<BoxCollider> other) override;

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
    bool hasDescripting = true;
    bool isDescripting = true;

    bool isUnlocked = true;
    static int lastItemIndex;
private:
    ItemType ItemIndex;
    std::shared_ptr<UIText> m_priceText;
    std::shared_ptr<Object> backgroundImage;

    float endTime=0;
    std::string nameText;
    std::shared_ptr<UIText> m_nameDescription;
    std::string effectText;
    std::shared_ptr<UIText> m_effectDescription;
    std::shared_ptr<Object> descriptionBackgroundImage;


    std::string UIeText = "PRESS E TO PURCHASE";
    std::shared_ptr<UIText> m_UIeDescription;
    std::shared_ptr<Object> UIeBackgroundImage;
};

#endif
