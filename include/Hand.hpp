#ifndef HAND_HPP
#define HAND_HPP

#include "pch.hpp"

#include "Util/Animation.hpp"
#include "core/Object.hpp"

class Player;

class Hand : public Object {
public:
    Hand();

    void Update();
    std::shared_ptr<Util::Animation> m_Animation;
    std::shared_ptr<Util::Animation> m_AnimationDash;
    std::weak_ptr<Player> m_Player;

protected:
};

#endif
