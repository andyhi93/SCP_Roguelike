#include "BloodCoin.hpp"
#include "random"
#include "core/ColliderManager.hpp"

BloodCoin::BloodCoin(glm::vec2 pos): Actor(glm::vec2{ 30,30 }){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 2);
    m_Transform.scale = { 2,2 };
    int cmd =dis(gen);
    if (cmd == 1) {
        isBig = true;
        value = 5;
        m_Transform.scale = { 3,3 };
    }
    this->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/items/bloodCoin.png"));

    m_collider->isTrigger = true;
    m_collider->tag = "Coin";
}
int BloodCoin::pickUpCoin() {
    isPick = true;
    return value;
}
void BloodCoin::Update() {

}
void BloodCoin::Start() {
    m_collider->parentActor = std::dynamic_pointer_cast<Object>(shared_from_this());
    if (!m_collider->parentActor) std::cout << "null coin\n";
    m_collider->position = m_Transform.translation;
    ColliderManager::GetInstance().RegisterCollider(m_collider);
}