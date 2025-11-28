#include "LifeUI.h"
#include"Externals/imgui/imgui.h"


LifeUI::LifeUI(Player* player){
	player_ = player;

	//必須となる情報の読み込み
	texture_.Load("./Resources/white.png");

	meter_.reset(MLEngine::Resource::Sprite::Create(texture_,plMeterPos_, plColor_));

	//必須となる情報の読み込み
	texture_.Load("./Resources/frame.png");

	frame_.reset(MLEngine::Resource::Sprite::Create(texture_, plFramePos_, plColor_));
}

void LifeUI::Initialize() {
	plMeterPos_ = { 60.0f,520.0f };
	plMeterSize_ = { 65.0f,520.0f };

	meter_->anchorPoint = { 0.5f,1.0f };

	frame_->anchorPoint = { 0.5f,1.0f };
}


void LifeUI::Update() {
	SizeCulc();

#ifdef _DEBUG
	DebugDraw();
#endif // _DEBUG
	meter_->position = plMeterPos_;	
	meter_->size = plMeterSize_;

	plFramePos_ = { plMeterPos_.x,plMeterPos_.y + 15.0f };
	plFrameSize_ = { plMeterSize_.x * 1.5f,PLyMax_ + 30.0f };

	frame_->position = plFramePos_;
	frame_->size = plFrameSize_;
}


void LifeUI::Draw() {

}


void LifeUI::DebugDraw() {
	ImGui::Begin("PlayerLifeTex");


	ImGui::DragFloat2("座標", &plMeterPos_.x, 0.5f, 0.0f, 1280.0f);

	ImGui::DragFloat2("サイズ", &plMeterSize_.x, 0.5f, 0.0f, 1280.0f);

	ImGui::DragFloat("縦の最大サイズ", &PLyMax_, 0.5f, 1.0f, 1280.0f);

	ImGui::End();
}

void LifeUI::SizeCulc(){
	//体力の最大値と現状の体力の割合から長さを算出
	float culcY = PLyMax_ * (static_cast<float>(player_->GetLife()) / player_->GetLifeMax());

	plMeterSize_.y = culcY;


}
