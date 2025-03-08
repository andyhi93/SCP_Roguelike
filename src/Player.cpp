#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include <iostream>

Player::Player(): Actor(glm::vec2{ 45,140 }){
    m_collider->tag = "Player";
    m_BulletBox = std::make_shared<BulletBox>();
    m_collider->SetTriggerCallback(std::make_shared<Trigger>());//for trigger func
    layer = CollisionLayer::Player;

    m_Transform.translation = { 0, 0 };
    m_AnimationIdle = std::make_shared<Util::Animation>(
        std::vector<std::string>{"../../../Resources/Player/boy_idle1.png","../../../Resources/Player/boy_idle2.png"}, true, 1000, true, 1000);
    m_AnimationWalk = std::make_shared<Util::Animation>(std::vector<std::string>{"../../../Resources/Player/boy_walk1.png", "../../../Resources/Player/boy_walk2.png",
        "../../../Resources/Player/boy_walk3.png", "../../../Resources/Player/boy_walk4.png",
        "../../../Resources/Player/boy_walk5.png", "../../../Resources/Player/boy_walk6.png",
        "../../../Resources/Player/boy_walk7.png", "../../../Resources/Player/boy_walk8.png", },true,50,true,50);
    m_AnimationDash= std::make_shared<Util::Animation>( std::vector<std::string>{"../../../Resources/Player/boy_dash.png"}, true, 1000, true, 1000);
    m_AnimationDie = std::make_shared<Util::Animation>(
        std::vector<std::string>{"../../../Resources/die_animation1.png", "../../../Resources/die_animation2.png"}, true, 50, true, 50);
    m_AnimationDie->SetLooping(false);
    SetDrawable(m_AnimationIdle);
    m_AnimationIdle->Play();
    m_Transform.scale = { 4,4 };
    m_Hand = std::make_shared<Hand>();
    this->AddChild(m_Hand);
    this->AddChild(m_BulletBox);
    //laterInit
    m_Hand->SetZIndex(this->GetZIndex() + 0.2f);
    m_BulletBox->SetZIndex(this->GetZIndex() + 0.1f);
}
void Player::Start() {
    //m_collider->parentActor = shared_from_this();
}
void Player::PlayerControl() {
    //Move
    glm::vec2 velocity(0.0f, 0.0f);

    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        currentState = Walk;
        velocity.y += speed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        currentState = Walk;
        velocity.x -= speed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        currentState = Walk;
        velocity.y -= speed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        currentState = Walk;
        velocity.x += speed;
    }
    if (!Util::Input::IsKeyPressed(Util::Keycode::W) &&  !Util::Input::IsKeyPressed(Util::Keycode::A) && 
        !Util::Input::IsKeyPressed(Util::Keycode::S) &&  !Util::Input::IsKeyPressed(Util::Keycode::D)) {
        currentState = Idle;
    }
    Move(velocity);
    //get mousePos
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_RB)) {
        LOG_INFO("Position:{} {}", Util::Input::GetCursorPosition().x, Util::Input::GetCursorPosition().y);
    }

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && (currentTime - m_LastShotTime >= m_ShotInterval)) {
        glm::vec2 bulletDirection = Util::Input::GetCursorPosition() - m_Transform.translation;
        auto bullet = std::make_shared<Bullet>(m_Transform.translation,1, CollisionLayer::Player, 10.0f, 0, bulletDirection);
        bullet->m_Transform.translation = m_Hand->m_Transform.translation;
        m_BulletBox->AddBullet(bullet);
        m_LastShotTime = currentTime;
    }
}
void Player::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
    if (other->tag == "Door") {
        glm::vec2 newPosition = m_Transform.translation + velocity;

        if (m_LevelManager->m_Tilemap->doors[0]->isOpen && velocity.x > 0) {
            m_LevelManager->ChangeRoom(glm::ivec2(1, 0));
            m_Transform.translation = glm::vec2(-793, -66);
        }
        if (m_LevelManager->m_Tilemap->doors[1]->isOpen && velocity.y < 0) {
            m_LevelManager->ChangeRoom(glm::ivec2(0, -1));
            m_Transform.translation = glm::vec2(-6, 240);
        }
        if (m_LevelManager->m_Tilemap->doors[2]->isOpen && velocity.x < 0) {
            m_LevelManager->ChangeRoom(glm::ivec2(-1, 0));
            m_Transform.translation = glm::vec2(802, -67);
        }
        if (m_LevelManager->m_Tilemap->doors[3]->isOpen && velocity.y > 0) {
            m_LevelManager->ChangeRoom(glm::ivec2(0, 1));
            m_Transform.translation = glm::vec2(-9, -397);
        }
    }
}
void Player::Move(glm::vec2& velocity) {
    MoveX(velocity.x);
    MoveY(velocity.y);

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (currentTime - lastDashEndTime >= dashCooldown) {
        canDash = true;
    }

    // 檢測是否按下 Shift 開始 Dash
    if (Util::Input::IsKeyDown(Util::Keycode::LSHIFT) && !isDashing && canDash) {
        isDashing = true;
        canDash = false;
        dashStartTime = currentTime;
    }

    // 若正在 Dash，則提升速度
    if (isDashing) {
        velocity *= dashSpeedMultiplier;
        if (currentTime - dashStartTime >= dashTime) {
            isDashing = false; 
            lastDashEndTime = currentTime; 
        }
    }

}
void Player::AnimationControl() {
    if (currentState == Die) {
        SetDrawable(m_AnimationDie);
        return;
    }
    if (isDashing) {
        SetDrawable(m_AnimationDash);
        m_Hand->SetDrawable(m_Hand->m_AnimationDash);
    }
    else if (currentState==Walk) {
        SetDrawable(m_AnimationWalk);
        m_Hand->SetDrawable(m_Hand->m_Animation);
    }
    else {
        SetDrawable(m_AnimationIdle);
        m_Hand->SetDrawable(m_Hand->m_Animation);
    }
    //flip
    if (m_Hand->m_Transform.rotation > 88.5f && m_Hand->m_Transform.rotation < 91.5f) {
        m_Transform.scale.x = std::abs(m_Transform.scale.x);
        m_Hand->m_Transform.scale.x = std::abs(m_Transform.scale.x);
        isFaceRight = true;
    }
    else {
        m_Transform.scale.x = -std::abs(m_Transform.scale.x);
        m_Hand->m_Transform.scale.x = -std::abs(m_Transform.scale.x);
        isFaceRight = false;
    }
}
void Player::HandControl() {
    glm::vec2 offset = isFaceRight ? glm::vec2(-12, 20) : glm::vec2(12, 20);
    m_Hand->m_Transform.translation = m_Transform.translation + offset;

    glm::vec2 mousePos = Util::Input::GetCursorPosition();
    glm::vec2 direction = mousePos - m_Transform.translation;
    float angle = isFaceRight ? atan2(direction.y, direction.x) : -atan2(direction.y, direction.x);
    m_Hand->m_Transform.rotation = isFaceRight ? angle + 90.0f : 180.0f - (angle + 90.0f);
}
void Player::Update() {
    m_Hand->Update();
    m_BulletBox->Update();
    //std::cout << "currentState: " << currentState<<std::endl;
    if (health <= 0 && currentState != Die) {
        currentState = Die;
        this->RemoveChild(m_Hand);
    }
    if (currentState != Die) {
        PlayerControl();
        HandControl();
    }
    AnimationControl();

    //Children object

}
