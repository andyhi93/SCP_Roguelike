//It's revolution!!!!!!!!!!!!REBUILD ALL COLLIDER
#ifndef BOXCOLLIDER_HPP
#define BOXCOLLIDER_HPP

#include <glm/glm.hpp>
#include "pch.hpp"
#include <iostream>
#include <unordered_set>
#include "Object.hpp"
class BoxCollider;

class Trigger {
public:
    bool isEnter = false;

    virtual void OnTriggerEnter(std::shared_ptr<BoxCollider> other) {}
    virtual void OnTriggerStay(std::shared_ptr<BoxCollider>  other) {}
    virtual void OnTriggerExit(std::shared_ptr<BoxCollider> other) {}
};

class BoxCollider: public std::enable_shared_from_this<BoxCollider> {
public:
    bool isTrigger = false;
    bool isSolid = false;//wall
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 offset = { 0,0 };
    std::string tag="";
    std::weak_ptr<Trigger> triggerCallback; // Ĳ�o�^��
    bool isActive = true;  // �O�_�ҥθ� Collider
    std::weak_ptr<Object> parentActor;

    BoxCollider(glm::vec2 _pos, glm::vec2 _size);

    void SetTriggerCallback(std::shared_ptr<Trigger> callback);

    void SetOffset(glm::vec2 offset);

    bool CheckCollision(std::shared_ptr<BoxCollider> other);
    bool CheckCollision(std::shared_ptr<BoxCollider> other, int cmdxy);
    bool CheckCollisionEdge(std::shared_ptr<BoxCollider> other);

    // �Ω�Ĳ�o�i�J�B���d�M�h�X�ƥ�
    void HandleCollision(std::shared_ptr<BoxCollider> other);

    virtual void OnCollisionEnter(std::shared_ptr<BoxCollider> other) {};
    virtual void OnCollisionExit(std::shared_ptr<BoxCollider> other) {};
private:
    std::unordered_set<std::shared_ptr<BoxCollider>> currentCollisions;
};


#endif