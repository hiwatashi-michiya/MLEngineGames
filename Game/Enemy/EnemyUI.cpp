#include "Enemy/EnemyUI.h"

#include <format>

#include "Enemy/Enemy.h"
#include"Externals/imgui/imgui.h"

void EnemyUI::Initialize(Enemy* enemy)
{
	global_ = GlobalVariables::GetInstance();

	enemy_ = enemy;

	barPosition_ = global_->GetVector2Value("EnemyUI", "HPBarPosition");
	barSize_ = global_->GetVector2Value("EnemyUI", "HPBarSize");

	framePosition_ = global_->GetVector2Value("EnemyUI", "FramePosition");
	frameSize_ = global_->GetVector2Value("EnemyUI", "FrameSize");

	//必須となる情報の読み込み
	texture_.Load("./Resources/white.png");

	hpBar_.reset(MLEngine::Resource::Sprite::Create(texture_, barPosition_, barColor_));
	//hpBar_->anchorPoint = { 0.0f,0.5f };

	//必須となる情報の読み込み
	texture_.Load("./Resources/frame.png");

	frame_.reset(MLEngine::Resource::Sprite::Create(texture_, framePosition_, frameColor_));
}

void EnemyUI::Update() {
	
	MLEngine::Math::Vector2 barSize = {barSize_.x * (static_cast<float>(enemy_->GetHp()) / static_cast<float>(enemy_->GetMaxHp())), barSize_.y };


	if (changeFlag_) {
		hpBar_->anchorPoint = { 0.0f,0.5f };

		hpBar_->position = { barPosition_.x - (barSize_.x / 2.0f), barPosition_.y };
		hpBar_->size = barSize;

		frame_->position = framePosition_;
		frame_->size = frameSize_;
	}
	else {
		hpBar_->anchorPoint = { 0.5f,0.5f };
		hpBar_->position = barPosition_;
		hpBar_->size = barSize;

		frame_->position = framePosition_;
		frame_->size = frameSize_;
	}


#ifdef _DEBUG
	ImGui::Begin("EnemyUI");
	ImGui::DragFloat2("体力バー座標", &barPosition_.x, 0.5f, 0.0f, 1280.0f);
	global_->datas_["EnemyUI"].items["HPBarPosition"].value = barPosition_;
	ImGui::DragFloat2("体力バーサイズ", &barSize_.x, 0.5f, 0.0f, 1280.0f);
	global_->datas_["EnemyUI"].items["HPBarSize"].value = barSize_;
	ImGui::DragFloat2("フレーム座標", &framePosition_.x, 0.5f, 0.0f, 1280.0f);
	global_->datas_["EnemyUI"].items["FramePosition"].value = framePosition_;
	ImGui::DragFloat2("フレームサイズ", &frameSize_.x, 0.5f, 0.0f, 1280.0f);
	global_->datas_["EnemyUI"].items["FrameSize"].value = frameSize_;

	ImGui::Checkbox("ゲージ減少変更モード", &changeFlag_);

	if (ImGui::Button("Save")) {
		global_->SaveFile("EnemyUI");
		std::string message = std::format("{}.json saved", "EnemyUI");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}
	ImGui::End();
#endif // _DEBUG
}