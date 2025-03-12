#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include <iostream>
#include <Table.hpp>

/*   [info] Position:-22 -53
 [info] Position:16 40*/
Player::Player(): Actor(glm::vec2{ 45,100 }){
    m_collider->tag = "Player";
    m_BulletBox = std::make_shared<BulletBox>();
    layer = CollisionLayer::Player;

    m_Transform.translation = { 0, 0 };
    m_AnimationIdle = std::make_shared<Util::Animation>(
        std::vector<std::string>{RESOURCE_DIR "/Player/boy_idle1.png", RESOURCE_DIR "/Player/boy_idle2.png"}, true, 1000, true, 1000);
    m_AnimationWalk = std::make_shared<Util::Animation>(std::vector<std::string>{RESOURCE_DIR "/Player/boy_walk1.png", RESOURCE_DIR "/Player/boy_walk2.png",
        RESOURCE_DIR "/Player/boy_walk3.png", RESOURCE_DIR "/Player/boy_walk4.png",
        RESOURCE_DIR "/Player/boy_walk5.png", RESOURCE_DIR "/Player/boy_walk6.png",
        RESOURCE_DIR "/Player/boy_walk7.png", RESOURCE_DIR "/Player/boy_walk8.png", },true,50,true,50);
    m_AnimationHurt = std::make_shared<Util::Animation>(std::vector<std::string>{RESOURCE_DIR "/Player/boy_hurt1.png", RESOURCE_DIR "/Player/boy_hurt2.png",
        RESOURCE_DIR "/Player/boy_hurt3.png", RESOURCE_DIR "/Player/boy_hurt4.png",
        RESOURCE_DIR "/Player/boy_hurt5.png", RESOURCE_DIR "/Player/boy_hurt6.png",
        RESOURCE_DIR "/Player/boy_hurt7.png", RESOURCE_DIR "/Player/boy_hurt8.png", }, true, 50, true, 50);
    m_AnimationDash= std::make_shared<Util::Animation>( std::vector<std::string>{RESOURCE_DIR "/Player/boy_dash.png"}, true, 1000, true, 1000);
    m_AnimationDie = std::make_shared<Util::Animation>(
        std::vector<std::string>{RESOURCE_DIR "/die_animation1.png", RESOURCE_DIR "/die_animation2.png"}, true, 50, true, 50);
    m_AnimationDie->SetLooping(false);
    SetDrawable(m_AnimationIdle);
    m_AnimationIdle->Play();
    m_Transform.scale = { 4,4 };
    m_Hand = std::make_shared<Hand>();
}
void Player::Start() {
    m_collider->SetTriggerCallback(std::dynamic_pointer_cast<Trigger>(shared_from_this()));//for trigger func
    m_collider->parentActor = shared_from_this();
    m_Hand->SetZIndex(this->GetZIndex() + 0.2f);
    m_BulletBox->SetZIndex(this->GetZIndex() - 0.1f);
    this->AddChild(m_Hand);
    this->AddChild(m_BulletBox);
}
void Player::PlayerControl() {
    //Move
    glm::vec2 velocity(0.0f, 0.0f);

    if (Util::Input::IsKeyPressed(Util::Keycode::W)) {
        currentState = (currentState == Hurt) ? Hurt : Walk;
        velocity.y = (velocity.y<speed)? velocity.y+speed : velocity.y;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::A)) {
        currentState = (currentState == Hurt) ? Hurt : Walk;
        velocity.x = (velocity.x > -speed) ? velocity.x - speed : velocity.x;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::S)) {
        currentState = (currentState == Hurt) ? Hurt : Walk;
        velocity.y = (velocity.y > -speed) ? velocity.y - speed : velocity.y;
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::D)) {
        currentState = (currentState == Hurt) ? Hurt : Walk;
        velocity.x = (velocity.x < speed) ? velocity.x + speed : velocity.x;
    }
    if (!Util::Input::IsKeyPressed(Util::Keycode::W) &&  !Util::Input::IsKeyPressed(Util::Keycode::A) && 
        !Util::Input::IsKeyPressed(Util::Keycode::S) &&  !Util::Input::IsKeyPressed(Util::Keycode::D)) {
        currentState = (currentState == Hurt) ? Hurt : Idle;
    }
    Move(velocity);
    //get mousePos
    if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_RB)) {
        LOG_INFO("Position:{} {}", Util::Input::GetCursorPosition().x, Util::Input::GetCursorPosition().y);
    }

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && (currentTime - m_LastShotTime >= m_ShotInterval)) {
        glm::vec2 bulletDirection = Util::Input::GetCursorPosition() - m_Transform.translation;
        auto bullet = std::make_shared<Bullet>(m_Transform.translation,1, CollisionLayer::Player, 7.0f, 0, bulletDirection);
        bullet->m_Transform.translation = m_Hand->m_Transform.translation;
        m_BulletBox->AddBullet(bullet);
        m_LastShotTime = currentTime;
    }
}
void Player::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
    if (other->tag == "Door0" && m_LevelManager->m_Tilemap->doors[0]->isOpen) {
        m_LevelManager->ChangeRoom(glm::ivec2(1, 0));
        m_Transform.translation = glm::vec2(-793, -66);
    }
    if (other->tag == "Door1" && m_LevelManager->m_Tilemap->doors[1]->isOpen) {
        m_LevelManager->ChangeRoom(glm::ivec2(0, -1));
        m_Transform.translation = glm::vec2(-6, 348);
    }
    if (other->tag == "Door2" && m_LevelManager->m_Tilemap->doors[2]->isOpen) {
        m_LevelManager->ChangeRoom(glm::ivec2(-1, 0));
        m_Transform.translation = glm::vec2(802, -67);
    }
    if (other->tag == "Door3" && m_LevelManager->m_Tilemap->doors[3]->isOpen) {
        m_LevelManager->ChangeRoom(glm::ivec2(0, 1));
        m_Transform.translation = glm::vec2(-9, -397);
    }
    if (other->tag == "Table") {
        std::shared_ptr<Table> table = std::dynamic_pointer_cast<Table>(other->parentActor);
        if(!table->isBroken) table->BreakTable();
    }
    if (!isDashing && currentState!=Hurt) {
        if (other->tag == "Trap") {
            Damage(1.0f);
        }
    }
    if (other->tag == "Coin") {
        std::shared_ptr<BloodCoin> coin = std::dynamic_pointer_cast<BloodCoin>(other->parentActor);
        if (coin && !coin->isPick) {
            std::cout << "Pick up Coin\n";
            coinAmount += coin->pickUpCoin();
            coin->m_collider->tag = "PickedCoin";
            coin = nullptr;
            other = nullptr;
        }
    }
}
void Player::Damage(float damage) {
    if (currentState != Hurt) {
        SetHealth(GetHealth() - damage);
        m_collider->isActive = false;
        currentState = Hurt;
    }
}
void Player::SetHealth(float amount) { health = amount; }
float Player::GetHealth() { return health; }
void Player::Move(glm::vec2& velocity) {

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (currentTime - lastDashEndTime >= dashCooldown) {
        canDash = true;
    }

    // 檢測是否按下 Shift 開始 Dash
    if (Util::Input::IsKeyDown(Util::Keycode::LSHIFT) && !isDashing && canDash) {
        std::vector<std::shared_ptr<BoxCollider>> tables= ColliderManager::GetInstance().GetTableColliders();
        for(auto& table : tables){
            if(table->tag=="Table") table->isTrigger = true;
        }
        isDashing = true;
        canDash = false;
        dashStartTime = currentTime;
    }

    // 若正在 Dash，則提升速度
    if (isDashing) {
        velocity *= dashSpeedMultiplier;
        if (currentTime - dashStartTime >= dashTime) {
            std::vector<std::shared_ptr<BoxCollider>> tables = ColliderManager::GetInstance().GetTableColliders();
            for (auto& table : tables) {
                if (table->tag == "Table") table->isTrigger = false;
            }
            isDashing = false;
            lastDashEndTime = currentTime; 
        }
    }
    MoveX(velocity.x);
    MoveY(velocity.y);
    //std::cout << "Player pos: " << m_Transform.translation.x<<", "<< m_Transform.translation.y << " Collider pos: " << m_collider->position.x<<", "<< m_collider->position.y << std::endl;
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
    else if (currentState == Hurt) {
        SetDrawable(m_AnimationHurt);
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
void Player::FixedUpdate() {
    m_BulletBox->FixedUpdate();
}
void Player::Update() {
    m_Hand->Update();
    m_BulletBox->Update();
    if (health <= 0 && currentState != Die) {
        currentState = Die;
        this->RemoveChild(m_Hand);
    }
    if (currentState != Die) {
        if (currentState == Hurt) {
            float currentTime = SDL_GetTicks() / 1000.0f;
            if (!isInvincible) {
                isInvincible = true;
                lastHurtTime = currentTime;
            }
            if (isInvincible && currentTime - lastHurtTime >= isInvincible) {
                currentState = Idle;
                isInvincible = false;
                m_collider->isActive = true;
            }
        }
        PlayerControl();
        HandControl();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        for (auto& door : m_LevelManager->m_Tilemap->doors) {
            door->DoorControl(true);
        }
    }
    AnimationControl();

    //Children object

}
