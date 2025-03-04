#include "SCP610.hpp"
#include "Player.hpp"
SCP610::SCP610() : Enemy(){
	speed = 100.0f;
	m_Transform.translation = { 50,0 };
	m_Collider = std::make_shared<Collider>(m_Transform.translation, glm::vec2{ 45,140 });
	m_AnimationWalk = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP610/scp610_walk1.png", "../../../Resources/SCP610/scp610_walk2.png", 
		"../../../Resources/SCP610/scp610_walk3.png", "../../../Resources/SCP610/scp610_walk4.png", }, true, 100, true, 100);
	m_AnimationAttack = std::make_shared<Util::Animation>(
		std::vector<std::string>{"../../../Resources/SCP610/scp610_attack1.png", "../../../Resources/SCP610/scp610_attack2.png",
		"../../../Resources/SCP610/scp610_attack3.png", "../../../Resources/SCP610/scp610_attack4.png", }, true, 100, true, 100);
	SetDrawable(m_AnimationWalk);
	m_AnimationWalk->Play();
	m_Transform.scale = { 5,5 };
}
void SCP610::Update() {
	m_Collider->position = m_Transform.translation;

	glm::vec2 PlayerPos = m_Player->m_Transform.translation;
	glm::vec2 direction = PlayerPos - m_Transform.translation;
	glm::vec2 velocity(0.0f, 0.0f);
	velocity =normalize(direction);
	velocity = { velocity.x * speed,velocity.y * speed };
	m_Transform.translation+=m_Collider->blockDetect(velocity);
	FlipControl();
}