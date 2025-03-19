#ifndef MENU_HPP
#define MENU_HPP

#include "pch.hpp"

#include "core/Object.hpp"
#include "Button.hpp"
#include "Util/Animation.hpp"

class Menu : public Object {
public:
    Menu();

    void OpenMenu();
    void CloseMenu();

    void Update();

    bool isStartMenu = true;
    std::shared_ptr<Button> startButton;
    std::shared_ptr<Button> exitButton;
    std::shared_ptr<Util::Animation> menuAnimation;

    std::shared_ptr<Button> resumeButton;
    std::shared_ptr<Button> menuButton;
    std::shared_ptr<Util::Image> stopImage;
private:
};

#endif
