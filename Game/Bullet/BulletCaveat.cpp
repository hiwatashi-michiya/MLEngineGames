#include "BulletCaveat.h"

#include <format>

#include"Externals/imgui/imgui.h"
#include "Player/Player.h"

void BulletCaveat::Initialize(Player* player)
{
	player_ = player;

	global_ = GlobalVariables::GetInstance();

	size_ = global_->GetVector2Value("BulletCaveat", "Size");
	position_ = global_->GetVector2Value("BulletCaveat", "Position");
	distance_ = global_->GetFloatValue("BulletCaveat", "Distance");
	switchTime_ = global_->GetFloatValue("BulletCaveat", "SwitchTime");
	totalTime_ = global_->GetFloatValue("BulletCaveat", "TotalTime");

	caveatSprites_.clear();

	MLEngine::Resource::Texture texture;
	texture.Load("./Resources/Texture/ingame_UI_arart.png");
	for (int i = 0; i < 3; ++i) {
		std::unique_ptr<MLEngine::Resource::Sprite2D> caveatSprite(MLEngine::Resource::Sprite2D::Create(
				texture,
				MLEngine::Math::Vector2(position_.x + distance_ * (i - 1), position_.y),
				{ 1.0f, 1.0f, 1.0f, 1.0f }
			)
		);
		caveatSprite->isActive = false;
		caveatSprite->size = size_;
		caveatSprites_.push_back(std::move(caveatSprite));
	}

	effectElements_.clear();
}

void BulletCaveat::Update()
{


	for (auto& effectElement : effectElements_) {
		effectElement.time += 1.0f / 60.0f;

		// 
		int currentIndex = static_cast<int>(effectElement.time / effectElement.switchTime);

		// 表示切り替え
		if (currentIndex != effectElement.prevSwitchIndex) {
			caveatSprites_[effectElement.lane]->isActive ^= true;
			effectElement.prevSwitchIndex = currentIndex;
		}

	}

	effectElements_.remove_if([this](const EffectElement& effectElement) {
		if (effectElement.time > effectElement.totalTime) {
			return true;
		}
		return false;
		});

	for (int i = 0; i < 3; i++) {

		if (!player_->GetIsForward()) {
			caveatSprites_[i]->isActive = false;
		}


#ifdef _DEBUG
		caveatSprites_[i]->position = MLEngine::Math::Vector2(position_.x + distance_ * (i - 1), position_.y);
		caveatSprites_[i]->size = size_;
#endif // _DEBUG
	}


}


void BulletCaveat::DebugUI() {

#ifdef _DEBUG
	ImGui::Begin("EnemyCaveat");
	ImGui::DragFloat2("座標", &position_.x, 0.5f, 0.0f, 1280.0f);
	global_->datas_["BulletCaveat"].items["Position"].value = position_;
	ImGui::DragFloat2("サイズ", &size_.x, 0.5f, 0.0f, 1280.0f);
	global_->datas_["BulletCaveat"].items["Size"].value = size_;
	ImGui::DragFloat("間隔", &distance_, 10.0f);
	global_->datas_["BulletCaveat"].items["Distance"].value = distance_;
	ImGui::DragFloat("点滅間隔", &switchTime_, 0.01f, 0.0f);
	global_->datas_["BulletCaveat"].items["SwitchTime"].value = switchTime_;
	ImGui::DragFloat("表示時間", &totalTime_, 0.01f, 0.0f);
	global_->datas_["BulletCaveat"].items["TotalTime"].value = totalTime_;


	if (ImGui::Button("Save")) {
		global_->SaveFile("BulletCaveat");
		std::string message = std::format("{}.json saved", "BulletCaveat");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}

	ImGui::End();
#endif // _DEBUG

}

void BulletCaveat::Warn(int lane)
{
	if(lane < 0 || lane > 2){
		return;
	}
	EffectElement effectElement = { 0.0f, switchTime_, totalTime_, lane , 0 };

	effectElements_.push_back(effectElement);
}
