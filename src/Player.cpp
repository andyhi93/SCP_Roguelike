#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include <iostream>

Player::Player()
    :m_BulletBox(std::make_shared<BulletBox>()){
    layer = GameObject::CollisionLayer::Player;

    m_Collider = std::make_shared<Collider>(m_Transform.translation, glm::vec2{ 45,140 });
    m_Transform.translation = { 0, 0 };
    m_AnimationIdle = std::make_shared<Util::Animation>(
        std::vector<std::string>{"../../../Resources/Player/boy_idle1.png","../../../Resources/Player/boy_idle2.png"}, true, 1000, true, 1000);
    m_AnimationWalk = std::make_shared<Util::Animation>(std::vector<std::string>{"../../../Resources/Player/boy_walk1.png", "../../../Resources/Player/boy_walk2.png",
        "../../../Resources/Player/boy_walk3.png", "../../../Resources/Player/boy_walk4.png",
        "../../../Resources/Player/boy_walk5.png", "../../../Resources/Player/boy_walk6.png",
        "../../../Resources/Player/boy_walk7.png", "../../../Resources/Player/boy_walk8.png", },true,50,true,50);
    m_AnimationDash= std::make_shared<Util::Animation>( std::vector<std::string>{"../../../Resources/Player/boy_dash.png"}, true, 1000, true, 1000);
    SetDrawable(m_AnimationIdle);
    m_AnimationIdle->Play();
    m_Transform.scale = { 4,4 };
    m_Hand = std::make_shared<Hand>();
}
void Player::getLevelManager(std::shared_ptr<LevelManager> _LevelManager){
    m_LevelManager = _LevelManager;
}
void Player::PlayerControl() {
    //Move
    glm::vec2 velocity(0.0f, 0.0f);

    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        isMoving = true;
        velocity.y += speed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        isMoving = true;
        velocity.x -= speed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        isMoving = true;
        velocity.y -= speed;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        isMoving = true;
        velocity.x += speed;
    }
    if (!Util::Input::IsKeyPressed(Util::Keycode::W) && 
        !Util::Input::IsKeyPressed(Util::Keycode::A) && 
        !Util::Input::IsKeyPressed(Util::Keycode::S) && 
        !Util::Input::IsKeyPressed(Util::Keycode::D)) {
        isMoving = false;
    }
    Move(velocity);
    //get mousePos
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_RB)) {
        LOG_INFO("Position:{} {}", Util::Input::GetCursorPosition().x, Util::Input::GetCursorPosition().y);
    }

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && (currentTime - m_LastShotTime >= m_ShotInterval)) {
        glm::vec2 bulletDirection = glm::normalize(Util::Input::GetCursorPosition() - m_Transform.translation);
        auto bullet = std::make_shared<Bullet>(1, CollisionLayer::Player, 10.0f, 0, bulletDirection);
        bullet->m_Transform.translation = m_Hand->m_Transform.translation;
        m_BulletBox->AddBullet(bullet);
        m_LastShotTime = currentTime;
    }
}
void Player::Move(glm::vec2& velocity) {
    const std::vector<Collider>& walls = Collider::GetWalls();//tblr
    const std::vector<Collider>& doors = Collider::GetDoors();//eswn
    glm::vec2 newPosition = m_Transform.translation + velocity;

    if (m_Collider->CheckCollision(*m_Collider, doors[0]) && velocity.x > 0) {
        if (m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            m_LevelManager->ChangeRoom(glm::ivec2(1, 0));
            m_Transform.translation = glm::vec2(-793 ,- 66);
        }
        else velocity.x = 0;
    }
    if (m_Collider->CheckCollision(*m_Collider, doors[1])  && velocity.y < 0 ) {
        if (m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            m_LevelManager->ChangeRoom(glm::ivec2(0, -1));
            m_Transform.translation = glm::vec2(-6 ,240);
        }
        else velocity.y = 0;
    }
    if (m_Collider->CheckCollision(*m_Collider, doors[2]) && velocity.x < 0 ){
        if (m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            m_LevelManager->ChangeRoom(glm::ivec2(-1, 0));
            m_Transform.translation = glm::vec2(802, -67);
        }
        else velocity.x = 0;
    }
    if (m_Collider->CheckCollision(*m_Collider, doors[3]) && velocity.y > 0 ) {
        if (m_LevelManager->m_Tilemap->doors[0]->isOpen) {
            m_LevelManager->ChangeRoom(glm::ivec2(0, 1));
            m_Transform.translation = glm::vec2(-9 ,- 397);
        }
        else velocity.y = 0;
    }
    //wall
    if ((m_Collider->CheckCollision(*m_Collider, walls[0]) || m_Collider->CheckCollision(*m_Collider, walls[1])) && velocity.x > 0) {
        velocity.x = 0;
    }
    if ((m_Collider->CheckCollision(*m_Collider, walls[2]) || m_Collider->CheckCollision(*m_Collider, walls[3])) && velocity.y < 0) {
        velocity.y = 0;
    }
    if ((m_Collider->CheckCollision(*m_Collider, walls[4]) || m_Collider->CheckCollision(*m_Collider, walls[5])) && velocity.x < 0) {
        velocity.x = 0;
    }
    if ((m_Collider->CheckCollision(*m_Collider, walls[6]) || m_Collider->CheckCollision(*m_Collider, walls[7])) && velocity.y > 0) {
        velocity.y = 0;
    }

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

    m_Transform.translation += velocity;

}
void Player::AnimationControl() {
    if (isDashing) {
        SetDrawable(m_AnimationDash);
        m_Hand->SetDrawable(m_Hand->m_AnimationDash);
    }
    else if (isMoving) {
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
    PlayerControl();
    AnimationControl();
    HandControl();
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        for (auto& door : m_LevelManager->m_Tilemap->doors) {
            door->DoorControl(true);
        }
    }
    m_Collider->position = m_Transform.translation+glm::vec2(-22.5f,-70);

    //Children object

}
