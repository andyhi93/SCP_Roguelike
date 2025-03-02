#ifndef HAND_HPP
#define HAND_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"

class Player;

class Hand : public Util::GameObject {
public:
    Hand();

    void Update();
    std::shared_ptr<Util::Animation> m_Animation;
    std::shared_ptr<Util::Animation> m_AnimationDash;
    std::shared_ptr<Player> m_Player;

protected:
};

#endif
