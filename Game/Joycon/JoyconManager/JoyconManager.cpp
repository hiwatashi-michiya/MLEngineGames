#include "JoyconManager.h"

void JoyconManager::Init() {
	joyconL = std::make_unique<JoyconL>();
	joyconR = std::make_unique<JoyconR>();
	joyconL->Init(JoyconType::JOYCON_L);
	joyconR->Init(JoyconType::JOYCON_R);
}

void JoyconManager::Update() {
	joyconL->Update();
	joyconR->Update();
	CheakRadius();
}

void JoyconManager::Draw() {
#ifdef _DEBUG
	joyconL->ImGui("joyconL");
	joyconR->ImGui("joyconR");
	ImGui::Begin("JoyconManager");
	if (direction_ == front) {
		ImGui::Text("front");
	}else if (direction_ == back) {
		ImGui::Text("back");
	}
	else {
		ImGui::Text("no");
	}
	ImGui::End();
#endif // _DEBUG
}

direction JoyconManager::CheakRadius() {
	direction Ldire = joyconL->CheakRadius();
	direction Rdire = joyconR->CheakRadius();

	if(Rdire == front && Ldire ==Right) {
		direction_ = direction::front;
		return direction::front;
	}
	else if(Rdire == back && Ldire == Left) {
		direction_ = direction::back;
		return direction::back;
	}
	else {
		return direction_;
	}
}