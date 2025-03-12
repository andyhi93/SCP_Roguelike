#ifndef BLOODCOIN_HPP
#define BLOODCOIN_HPP

#include "pch.hpp"

#include "Util/Image.hpp"
#include "Core/Actor.hpp"

class BloodCoin : public Actor, public Trigger {
public:
    BloodCoin(glm::vec2 pos);

    void Start();
    void Update() override;
    int pickUpCoin();
    bool isPick = false;
private:
    bool isBig = false;
    int value = 1;
};

#endif
