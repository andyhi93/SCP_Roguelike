#ifndef ITEM_HPP
#define ITEM_HPP

#include "pch.hpp"
#include "Charater.hpp"
#include "Collider.hpp"

class Item : public Charater {
public:
    Item();

    void Update();
    enum ItemIndex {
        redPill,
        redClot,
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
private:
    int ItemType;
};

#endif
