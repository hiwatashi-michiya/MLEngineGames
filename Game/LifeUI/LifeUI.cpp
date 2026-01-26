#include "LifeUI.h"
#include"Externals/imgui/imgui.h"


LifeUI::LifeUI(Player* player){
	GlobalVariables* global = GlobalVariables::GetInstance();

	global->SetValue("UIState", "PLLifeColor", plColor_);


	player_ = player;

	//必須となる情報の読み込み
	texture_.Load("./Resources/white.png");

	meter_.reset(MLEngine::Resource::Sprite2D::Create(texture_,plMeterPos_, plColor_));
	//必須となる情報の読み込み
	texture_.Load("./Resources/frame.png");

	frame_.reset(MLEngine::Resource::Sprite2D::Create(texture_, plFramePos_, plColor_));


}

void LifeUI::Initialize() {
	plMeterPos_ = { 60.0f,568.0f };
	plMeterSize_ = { 45.0f,223.0f };

	meter_->anchorPoint = { 0.5f,1.0f };

	frame_->anchorPoint = { 0.5f,1.0f };


	plFrameSize_ = { 100.0f,400.0f };
}


void LifeUI::Update() {
	SizeCulc();

	GlobalVariables* global = GlobalVariables::GetInstance();
	plColor_ = global->GetVector4Value("UIState", "PLLifeColor");

#ifdef _DEBUG
	DebugDraw();
#endif // _DEBUG
	meter_->color = plColor_;
	meter_->position = plMeterPos_;	
	meter_->size = plMeterSize_;

	plFramePos_ = { plMeterPos_.x,plMeterPos_.y + 20.0f };
	//plFrameSize_ = { plMeterSize_.x * 1.5f,PLyMax_ + 30.0f };

	frame_->position = plFramePos_;
	frame_->size = plFrameSize_;
}


void LifeUI::Draw() {

}


void LifeUI::DebugDraw() {
	ImGui::Begin("PlayerLifeTex");


	ImGui::DragFloat2("座標", &plMeterPos_.x, 0.5f, 0.0f, 1280.0f);

	ImGui::DragFloat2("メーターサイズ", &plMeterSize_.x, 0.5f, 0.0f, 1280.0f);
	ImGui::DragFloat2("フレームサイズ", &plFrameSize_.x, 0.5f, 0.0f, 1280.0f);

	ImGui::DragFloat("縦の最大サイズ", &PLyMax_, 0.5f, 1.0f, 1280.0f);

	ImGui::End();
}

void LifeUI::SizeCulc(){
	//体力の最大値と現状の体力の割合から長さを算出
	float culcY = PLyMax_ * (static_cast<float>(player_->GetLife()) / player_->GetLifeMax());

	plMeterSize_.y = culcY;


}
