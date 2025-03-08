//It's revolution!!!!!!!!!!!!REBUILD ALL COLLIDER
#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include <glm/glm.hpp>
#include "pch.hpp"
#include <iostream>
#include <unordered_set>

class Actor;
class BoxCollider;

class Trigger {
public:
    bool isEnter = false;

    virtual void OnTriggerEnter(std::shared_ptr<BoxCollider> other) {}
    virtual void OnTriggerStay(std::shared_ptr<BoxCollider>  other) {}
    virtual void OnTriggerExit(std::shared_ptr<BoxCollider> other) {}
};

class BoxCollider {
public:
    bool isTrigger = false;
    bool isSolid = false;//wall
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 offset = { 0,0 };
    std::string tag;
    std::shared_ptr<Trigger> triggerCallback = nullptr; // 觸發回調
    bool isActive = true;  // 是否啟用該 Collider
    std::shared_ptr<Actor> parentActor;

    BoxCollider(glm::vec2 _pos, glm::vec2 _size);

    void SetTriggerCallback(std::shared_ptr<Trigger> callback);

    bool CheckCollision(std::shared_ptr<BoxCollider> other);

    // 用於觸發進入、停留和退出事件
    std::unordered_set<std::shared_ptr<BoxCollider>> currentCollisions;
    void HandleCollision(std::shared_ptr<BoxCollider> other);

    virtual void OnCollisionEnter(std::shared_ptr<BoxCollider> other) {};
    virtual void OnCollisionExit(std::shared_ptr<BoxCollider> other) {};
};


#endif