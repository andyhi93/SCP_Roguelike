#ifndef CHEST_HPP
#define CHEST_HPP

#include "Item.hpp"
#include "Core/Solid.hpp"
#include "Core/Actor.hpp"

class Chest : public Solid,public Trigger {
public:
    Chest(glm::vec2 pos, glm::vec2 size);

    void Start();
    void Update() override;
    void Open();
    void SetActive(bool IsActive);

    void OnTriggerEnter(std::shared_ptr<BoxCollider> other) override;
    void OnTriggerExit(std::shared_ptr<BoxCollider> other) override;

    bool isOpen = false;
    std::shared_ptr<Item> treasure;
private:
    std::shared_ptr<Actor> openArea;


    std::string UIeText = "PRESS E TO OPEN";
    std::shared_ptr<UIText> m_UIeDescription;
    std::shared_ptr<Object> UIeBackgroundImage;
};

#endif
