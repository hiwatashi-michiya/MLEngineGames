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
}

void JoyconManager::Draw() {
#ifdef _DEBUG
	joyconL->ImGui("joyconL");
	joyconR->ImGui("joyconR");
	ImGui::Begin("JoyconManager");
	ImGui::SetWindowFontScale(2.0f);
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
	direction Ldir = joyconL->CheakRadius();
	direction Rdir = joyconR->CheakRadius();

#ifdef _DEBUG
	Draw();
#endif // _DEBUG


	if(Rdir == front && Ldir == Right) {
		direction_ = direction::front;
		//joyconL->ResetRotate(Right);
		return direction::front;
	}
	else if(Rdir == back && Ldir == Left) {
		direction_ = direction::back;
		//joyconL->ResetRotate(Left);
		return direction::back;
	}
	else {
		return direction_;
	}
}