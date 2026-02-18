#include "JoyconManager.h"

void JoyconManager::Init() {
	joyconL = std::make_unique<Joycon>();
	joyconR = std::make_unique<Joycon>();
	joyconL->Init(JoyconType::JOYCON_L);
	joyconR->Init(JoyconType::JOYCON_R);
}

void JoyconManager::Update() {
	joyconL->Update();
	joyconR->Update();
	joyconL->CheakRadius();
	joyconR->CheakRadius();
}

void JoyconManager::Draw() {
	joyconL->ImGui("joyconL");
	joyconR->ImGui("joyconR");
}