#include "Menu.hpp"
#include <iostream>

//glm::vec2 pos, glm::vec2 size, float scaleSize, std::vector<std::shared_ptr<std::string>> path, int eventIndex
Menu::Menu(){
	startButton = std::make_shared<Button>(glm::vec2{ -500,-100 }, glm::vec2{400,180},3, std::vector<std::string>{RESOURCE_DIR "/UI/playButton1.png", RESOURCE_DIR "/UI/playButton2.png"},1);
	exitButton = std::make_shared<Button>(glm::vec2{ 500,-100 }, glm::vec2{ 400,180 }, 3, std::vector<std::string>{RESOURCE_DIR "/UI/exitButton1.png", RESOURCE_DIR "/UI/exitButton2.png"}, 0);
	menuAnimation = std::make_shared<Util::Animation>(std::vector<std::string>{RESOURCE_DIR "/UI/cover1.png", RESOURCE_DIR "/UI/cover2.png", RESOURCE_DIR "/UI/cover3.png", RESOURCE_DIR "/UI/cover4.png",
		RESOURCE_DIR "/UI/cover5.png", RESOURCE_DIR "/UI/cover6.png", RESOURCE_DIR "/UI/cover7.png", RESOURCE_DIR "/UI/cover8.png", }, true, 200, true, 200);
	m_Transform.scale = { 4,4 };
	this->SetZIndex(12);
	SetDrawable(menuAnimation);
	startButton->SetZIndex(13);
	exitButton->SetZIndex(13);
	this->AddChild(startButton);
	this->AddChild(exitButton);

	resumeButton=std::make_shared<Button>(glm::vec2{ 0,-200 }, glm::vec2{ 400,180 }, 3, std::vector<std::string>{RESOURCE_DIR "/UI/resumeButton1.png", RESOURCE_DIR "/UI/resumeButton2.png"}, 1);
	menuButton=std::make_shared<Button>(glm::vec2{ 0,-400 }, glm::vec2{ 400,180 }, 3, std::vector<std::string>{RESOURCE_DIR "/UI/menuButton1.png", RESOURCE_DIR "/UI/menuButton2.png"}, 1);
	stopImage = std::make_shared<Util::Image>(RESOURCE_DIR "/UI/stopMenu.png");
	this->AddChild(resumeButton);
	this->AddChild(menuButton);
	resumeButton->SetZIndex(13);
	menuButton->SetZIndex(13);
	resumeButton->isActive = false;
	menuButton->isActive = false;
	resumeButton->SetVisible(false);
	menuButton->SetVisible(false);


	//Fade
	FadeImage->SetVisible(false);
	m_AnimationFade = std::make_shared<Util::Animation>(std::vector<std::string>{RESOURCE_DIR "/UI/fade1.png", RESOURCE_DIR "/UI/fade2.png", RESOURCE_DIR "/UI/fade3.png",
		RESOURCE_DIR "/UI/fade4.png", RESOURCE_DIR "/UI/fade5.png", RESOURCE_DIR "/UI/fade6.png", RESOURCE_DIR "/UI/fade7.png", }, false, 100, false, 100);
	FadeImage->m_Transform.scale = { 4,4 };
	FadeImage->SetZIndex(15);
	FadeImage->SetDrawable(m_AnimationFade);
	this->AddChild(FadeImage);
}
void Menu::OpenMenu() {
	this->SetVisible(true);
	if (isStartMenu) {
		std::cout << "start menu\n";
		startButton->SetVisible(true);
		exitButton->SetVisible(true);
		SetDrawable(menuAnimation);
		startButton->isActive = true;
		exitButton->isActive = true;
		resumeButton->SetVisible(false);
		menuButton->SetVisible(false);
	}
	else {
		std::cout << "stop menu\n";
		resumeButton->SetVisible(true);
		menuButton->SetVisible(true);
		resumeButton->isActive = true;
		menuButton->isActive = true;
		SetDrawable(stopImage);
		startButton->SetVisible(false);
		exitButton->SetVisible(false);
	}
}
void Menu::CloseMenu() {
	if (isStartMenu) {
		startButton->isActive = false;
		startButton->SetVisible(false);
		exitButton->isActive = false;
		exitButton->SetVisible(false);
	}
	else {
		resumeButton->isActive = false;
		resumeButton->SetVisible(false);
		menuButton->isActive = false;
		menuButton->SetVisible(false);
	}
	this->SetVisible(false);
}
void Menu::Update() {
	if (isStartMenu) {
		startButton->Update();
		exitButton->Update();
	}
	else {
		resumeButton->Update();
		menuButton->Update();
	}
	//Fade
	if (isFading) {
		//std::cout << "FrameIndex: " << m_AnimationFade->GetCurrentFrameIndex()<<" State: " << int(m_AnimationFade->GetState()) << "\n";
		if (m_AnimationFade->GetCurrentFrameIndex() == 3) {
			isFading = false;
			isFullDark = true;
			m_AnimationFade->Pause();
		}
		if (m_AnimationFade->GetCurrentFrameIndex() == 6) {
			isFading = false;
			FadeImage->SetVisible(false);
		}
	}
}
void Menu::FadeIn() {
	FadeImage->SetVisible(true);
	std::cout << "FadeIn\n";
	isFading = true;
	m_AnimationFade->SetCurrentFrame(0);
	m_AnimationFade->Play();
	FadeImage->SetDrawable(m_AnimationFade);
}
void Menu::FadeOut() {
	std::cout << "FadeOut\n";
	isFullDark = false;
	isFading = true;
	m_AnimationFade->SetCurrentFrame(4);
	m_AnimationFade->Play();
	FadeImage->SetDrawable(m_AnimationFade);
}