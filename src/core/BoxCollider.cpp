#include "Core/BoxCollider.hpp"
#include "Core/ColliderManager.hpp"
    
BoxCollider::BoxCollider(glm::vec2 _pos, glm::vec2 _size) : position(_pos), size(_size) {
}
void BoxCollider::SetTriggerCallback(std::shared_ptr<Trigger> callback) {//需要用到Trigger系列函式使用
    triggerCallback = callback;
}
bool BoxCollider::CheckCollision(std::shared_ptr<BoxCollider> other) {
    return (position.x - size.x / 2 < other->position.x + other->size.x / 2 &&
        position.x + size.x / 2 > other->position.x - other->size.x / 2 &&
        position.y - size.y / 2 < other->position.y + other->size.y / 2 &&
        position.y + size.y / 2 > other->position.y - other->size.y / 2);
}
bool BoxCollider::CheckCollision(std::shared_ptr<BoxCollider> other, int cmdxy) {
    if (cmdxy) {
        return position.y - size.y / 2 < other->position.y + other->size.y / 2 &&
            position.y + size.y / 2 > other->position.y - other->size.y / 2;
    }
    else {
        return position.x - size.x / 2 < other->position.x + other->size.x / 2 &&
            position.x + size.x / 2 > other->position.x - other->size.x / 2;
    }
}
bool BoxCollider::CheckCollisionEdge(std::shared_ptr<BoxCollider> other) {
    return (position.x - size.x / 2 <= other->position.x + other->size.x / 2 &&
        position.x + size.x / 2 >= other->position.x - other->size.x / 2 &&
        position.y - size.y / 2 <= other->position.y + other->size.y / 2 &&
        position.y + size.y / 2 >= other->position.y - other->size.y / 2);
}
void BoxCollider::SetOffset(glm::vec2 offset) {
    this->offset = offset;
    position = position + offset;
}
// 用於觸發進入、停留和退出事件
void BoxCollider::HandleCollision(std::shared_ptr<BoxCollider> other) {
    if (triggerCallback && (this->isTrigger ^other->isTrigger)) {
        bool isColliding = CheckCollision(other);
        bool wasColliding = currentCollisions.count(other) > 0;
        if (isColliding) {
            if (!wasColliding) {
                //std::cout <<this->tag <<" Enter trigger with: " << other->tag << std::endl;
                triggerCallback->OnTriggerEnter(other);
                currentCollisions.insert(other);
            }
            else {
                triggerCallback->OnTriggerStay(other);
            }
        }
        else if (wasColliding) {
            //std::cout << this->tag << " Exited trigger with : " << other->tag << std::endl;
            triggerCallback->OnTriggerExit(other);
            currentCollisions.erase(other);
        }
    }
}
    