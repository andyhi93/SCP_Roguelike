#include "Player.hpp"

#include "Util/Input.hpp"
#include "Util/Transform.hpp"
#include "Util/Logger.hpp"
#include <iostream>
#include <Table.hpp>
#include "Item.hpp"
#include <Chest.hpp>
#include <Trap.hpp>

Player::Player(): Actor(glm::vec2{ 45,60 }){
    m_collider->offset = { 0,-20 };
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

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB) && (currentTime - m_LastShotTime >= m_ShotInterval)) {
        m_SFX->LoadMedia(m_GunSFXpath);
        m_SFX->Play();
        glm::vec2 bulletDirection = GetCursorPosition() - m_Transform.translation;
        std::shared_ptr<Bullet> bullet;
        bullet = std::make_shared<Bullet>(m_Hand->m_Transform.translation + glm::vec2{ 0,-15 }, ammoDamage>1? ammoDamage*0.9:1, CollisionLayer::Player, 10.0f, 0, bulletDirection);
        bullet->m_Transform.scale = { 1+ammoDamage*0.2,1+ammoDamage*0.2 };
        bullet->m_WorldCoord = m_WorldCoord + glm::vec2{ 0,-15 };
        m_BulletBox->AddBullet(bullet);
        m_LastShotTime = currentTime;
    }
}
void Player::OnTriggerEnter(std::shared_ptr<BoxCollider> other) {
    if (other->tag == "Door0" && m_LevelManager.lock()->m_Tilemap->doors[0]->isOpen) {
        m_LevelManager.lock()->ChangeRoom(glm::ivec2(1, 0));
        m_BulletBox->ChangeRoom();
        m_Transform.translation = glm::vec2(-793, -66);
    }
    if (other->tag == "Door1" && m_LevelManager.lock()->m_Tilemap->doors[1]->isOpen) {
        m_LevelManager.lock()->ChangeRoom(glm::ivec2(0, -1));
        m_BulletBox->ChangeRoom();
        m_Transform.translation = glm::vec2(-6, 348);
    }
    if (other->tag == "Door2" && m_LevelManager.lock()->m_Tilemap->doors[2]->isOpen) {
        m_LevelManager.lock()->ChangeRoom(glm::ivec2(-1, 0));
        m_BulletBox->ChangeRoom();
        m_Transform.translation = glm::vec2(802, -67);
    }
    if (other->tag == "Door3" && m_LevelManager.lock()->m_Tilemap->doors[3]->isOpen) {
        m_LevelManager.lock()->ChangeRoom(glm::ivec2(0, 1));
        m_BulletBox->ChangeRoom();
        m_Transform.translation = glm::vec2(-9, -397);
    }
    if (other->tag == "Table") {
        std::shared_ptr<Table> table = std::dynamic_pointer_cast<Table>(other->parentActor.lock());
        if(!table->isBroken) table->BreakTable();
    }
    if (other->tag == "Enemy") {
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(other->parentActor.lock());
        if (enemy) enemy->Damage(ammoDamage);
    }
    if (other->tag == "Elevator") {
        isElevate = true;
        std::cout << "Elevatorrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr\n";
    }
}
void Player::OnTriggerStay(std::shared_ptr<BoxCollider> other) {
    if (!other) return;


    if (!isDashing && currentState != Hurt) {
        if (other->tag == "Trap") {
            std::shared_ptr<Trap> trap = std::dynamic_pointer_cast<Trap>(other->parentActor.lock());
            if (trap && trap->isOpen) Damage(1.0f);
        }
    }
    if (other->tag == "Item") {
        std::shared_ptr<Item> item = std::dynamic_pointer_cast<Item>(other->parentActor.lock());
        if (!item) return;
        if (Util::Input::IsKeyDown(Util::Keycode::E) && !item->isUnlocked && coinAmount>=item->price) {
            std::cout << "buy\n";
            coinAmount -= item->price;
            item->isUnlocked = true;
        }
        if (!item->isPick && item->isUnlocked) {
            auto itemData = item->pickUp();
            std::cout << "itemIspick: " << item->isPick<<"\n";
            m_ShotInterval *= itemData[0];
            speed *= itemData[1];
            ammoDamage *= itemData[2];
            if (itemData[2] != 1) {
                float tempHealth = maxHealth * itemData[3];
                tempHealth = tempHealth - maxHealth;
                currentHealth += tempHealth;
                maxHealth *= itemData[3];
            }
            dashCooldown *= itemData[4];
            currentHealth = (currentHealth+ itemData[5])>maxHealth? maxHealth:currentHealth+ itemData[5];
            if (item->GetItemType() == Item::bloodCoin) coinAmount += itemData[6];
            item = nullptr;
            other = nullptr;
        }
    }
    else if (other->tag == "ChestTrigger") {
        std::shared_ptr<Chest> chest = std::dynamic_pointer_cast<Chest>(other->parentActor.lock());
        if (!chest) return;
        if (Util::Input::IsKeyDown(Util::Keycode::E) && !chest->isOpen) {
            std::cout << "chest open\n";
            chest->Open();
        }
    }
}
void Player::Damage(float damage) {
    if (currentState != Hurt && currentHealth>0 && !isDashing && !isInvincible) {
        m_SFX->LoadMedia(m_DeadSFXpath);
        m_SFX->Play();
        SetHealth(GetCurrentHealth() - damage);
        currentState = Hurt;
    }
}
void Player::SetHealth(float amount) { currentHealth = amount; }
float Player::GetHealth() { return maxHealth; }
void Player::Move(glm::vec2& velocity) {

    float currentTime = SDL_GetTicks() / 1000.0f;
    if (currentTime - lastDashEndTime >= dashCooldown) {
        canDash = true;
    }

    // 檢測是否按下 Shift 開始 Dash
    if (Util::Input::IsKeyDown(Util::Keycode::LSHIFT) && !isDashing && canDash) {
        lastDashEndTime = currentTime;
        std::vector<std::shared_ptr<BoxCollider>> tables= ColliderManager::GetInstance().GetTableColliders();
        for(auto& table : tables){
            if(table->tag=="Table") table->isTrigger = true;
        }
        std::vector<std::shared_ptr<BoxCollider>> enemies = ColliderManager::GetInstance().GetEnemyColliders();
        for (auto& enemy : enemies) {
            if (enemy->tag == "Enemy") enemy->isTrigger = true;
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
            std::vector<std::shared_ptr<BoxCollider>> enemies = ColliderManager::GetInstance().GetEnemyColliders();
            for (auto& enemy : enemies) {
                if (enemy->tag == "Enemy") enemy->isTrigger = false;
            }
            isDashing = false;
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
    if (m_Hand->m_Transform.rotation > 0 && m_Hand->m_Transform.rotation < glm::radians(180.0f)) {
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

    glm::vec2 mousePos = GetCursorPosition();
    glm::vec2 direction = mousePos - m_Transform.translation;
    float angle = isFaceRight ? atan2(direction.y, direction.x) : -atan2(direction.y, direction.x);
    m_Hand->m_Transform.rotation = isFaceRight ? angle + (glm::radians(90.0f)) : glm::radians(90.0f)- angle;
    //std::cout << glm::degrees(m_Hand->m_Transform.rotation) << std::endl;
}
void Player::FixedUpdate() {
}
void Player::Update() {
    //std::cout << "WorldCoord: " << m_WorldCoord.x << " ," << m_WorldCoord.y << "\n"<<"PlayerPos: "<<m_Transform.translation.x<<" ,"<<m_Transform.translation.y<<"\n";
    m_Hand->Update();
    m_BulletBox->Update();
    //std::cout << "Reference Count: " << shared_from_this().use_count() << std::endl;
    if (currentHealth < 1 && currentState != Die) {
        m_SFX->LoadMedia(m_DeadSFXpath);
        m_SFX->Play();
        currentState = Die;
        isDead = true;
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
            }
        }
        PlayerControl();
        HandControl();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE)) {
        for (auto& door : m_LevelManager.lock()->m_Tilemap->doors) {
            door->DoorControl(true);
        }
    }
    AnimationControl();

    //Children object

}
