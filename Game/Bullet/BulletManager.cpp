#include "BulletManager.h"

#include <format>

#include "Input/Input.h"
#include"Externals/imgui/imgui.h"
#include "Enemy/Enemy.h"

void BulletManager::Initialize()
{
	global_ = GlobalVariables::GetInstance();

	bullets_.clear();

	/*MLEngine::Resource::Texture texture;
	texture.Load("./Resources/EngineResources/blockMask.png");*/

	/*for (int i = 0; i < 3; ++i) {
		std::unique_ptr<MLEngine::Resource::Sprite> startSprite;
		startSprite.reset(MLEngine::Resource::Sprite::Create(texture, { 0.0f, 0.0f }, { 1.0f,1.0f,1.0f,0.5f }));
		startSprite->anchorPoint = { 0.5f,0.5f };
		startSprites_.push_back(std::move(startSprite));

		std::unique_ptr<MLEngine::Resource::Sprite> targetSprite;
		targetSprite.reset(MLEngine::Resource::Sprite::Create(texture, {0.0f, 0.0f}, { 1.0f,1.0f,1.0f,0.5f }));
		targetSprite->anchorPoint = { 0.5f,0.5f };
		targetSprites_.push_back(std::move(targetSprite));
	}*/

	global_->AddItem("BulletParameters", "StartScale", startScale_);
	startScale_ = global_->GetVector3Value("BulletParameters", "StartScale");
	global_->AddItem("BulletParameters", "EndScale", endScale_);
	endScale_ = global_->GetVector3Value("BulletParameters", "EndScale");
	global_->AddItem("BulletParameters", "StartTranslate", startTranslate_);
	startTranslate_ = global_->GetVector3Value("BulletParameters", "StartTranslate");
	global_->AddItem("BulletParameters", "EndTranslate", endTranslate_);
	endTranslate_ = global_->GetVector3Value("BulletParameters", "EndTranslate");
	global_->AddItem("BulletParameters", "StartDistance", startDistance_);
	startDistance_ = global_->GetFloatValue("BulletParameters", "StartDistance");
	global_->AddItem("BulletParameters", "EndDistance", endDistance_);
	endDistance_ = global_->GetFloatValue("BulletParameters", "EndDistance");
	global_->AddItem("BulletParameters", "BulletDamege", bulletDamege_);
	bulletDamege_ = global_->GetIntValue("BulletParameters", "BulletDamege");

	startModels_.clear();
	endModels_.clear();

	for (int i = 0; i < 3; ++i) {
		std::unique_ptr<MLEngine::Resource::RigidModel> startModel = std::make_unique<MLEngine::Resource::RigidModel>();
		startModel->Initialize("./Resources/model/plane/plane.obj");
		startModel->worldMatrix = MLEngine::Math::MakeAffineMatrix(startScale_, { 0.0f, 0.0f, 0.0f, 1.0f }, startTranslate_);
		startModel->color = { 1.0f,0.0f,0.0f,0.5f };
		startModels_.push_back(std::move(startModel));

		std::unique_ptr<MLEngine::Resource::RigidModel> endModel = std::make_unique<MLEngine::Resource::RigidModel>();
		endModel->Initialize("./Resources/model/plane/plane.obj");
		endModel->worldMatrix = MLEngine::Math::MakeAffineMatrix(endScale_, { 0.0f, 0.0f, 0.0f, 1.0f }, endTranslate_);
		endModel->color = { 0.0f,1.0f,0.0f,0.5f };
		endModels_.push_back(std::move(endModel));
	}

	

}

void BulletManager::Update()
{
	MLEngine::Input::Manager* input = MLEngine::Input::Manager::GetInstance();

	// 弾の更新
	for(auto& bullet : bullets_)
	{
		bullet->Update();

	}

	// 死んだ弾をリストから削除
	bullets_.remove_if([this](const std::unique_ptr<Bullet>& bullet) {
		if (bullet->IsDead()) {

			
			if (player_->GetNowLine() != bullet->GetNowLine()) { // レーンが違う場合は当たらない
				return true;
			}


			if (player_->GetIsForward()) { // プレイヤーが前を向いている場合のみダメージを受ける
				player_->OnCollision(bulletDamege_);
			}
			else { // 敵がダメージを受ける
				enemy_->OnCollision(bulletDamege_);
			}

			return true;
		}
		return false;
	});

	// 始点・終点モデルの更新
	for (int i = 0; i < 3; ++i) {
		/*startSprites_[i]->SetPosition({ launchPosition_.x + startDistance_ * (i - 1), launchPosition_.y });
		startSprites_[i]->size = { minSize_, minSize_ };
		targetSprites_[i]->SetPosition({ launchPosition_.x + endDistance_ * (i - 1), endLine_ });
		targetSprites_[i]->size = { maxSize_, maxSize_ };*/

		startModels_[i]->worldMatrix = MLEngine::Math::MakeAffineMatrix(startScale_, { 0.0f, 0.0f, 0.0f, 1.0f }, { startTranslate_.x + startDistance_ * (i - 1), startTranslate_.y, startTranslate_.z });
		endModels_[i]->worldMatrix = MLEngine::Math::MakeAffineMatrix(endScale_, { 0.0f, 0.0f, 0.0f, 1.0f }, { endTranslate_.x + endDistance_ * (i - 1), endTranslate_.y, endTranslate_.z });

	}

#ifdef _DEBUG

	ImGui::Begin("弾パラメーター");

	// 間隔
	ImGui::DragFloat("始点D", &startDistance_, 0.1f);
	global_->datas_["BulletParameters"].items["StartDistance"].value = startDistance_;
	ImGui::DragFloat("終点D", &endDistance_, 0.01f);
	global_->datas_["BulletParameters"].items["EndDistance"].value = endDistance_;

	ImGui::DragFloat3("始点平行移動", &startTranslate_.x, 0.1f);
	global_->datas_["BulletParameters"].items["StartTranslate"].value = startTranslate_;
	ImGui::DragFloat3("終点平行移動", &endTranslate_.x, 0.1f);
	global_->datas_["BulletParameters"].items["EndTranslate"].value = endTranslate_;

	// サイズ
	float startSize = startScale_.x;
	ImGui::DragFloat("始点サイズ", &startSize, 0.01f);
	startScale_ = { startSize, startSize, 1.0f };
	global_->datas_["BulletParameters"].items["StartScale"].value = startScale_;

	float endSize = endScale_.x;
	ImGui::DragFloat("終点サイズ", &endSize, 0.01f);
	endScale_ = { endSize, endSize, 1.0f };
	global_->datas_["BulletParameters"].items["EndScale"].value = endScale_;

	ImGui::DragInt("弾ダメージ", &bulletDamege_, 1);
	global_->datas_["BulletParameters"].items["BulletDamege"].value = bulletDamege_;

	if (ImGui::Button("Save")) {
		global_->SaveFile("BulletParameters");
		std::string message = std::format("{}.json saved", "BulletParameters");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}

	ImGui::End();

#endif // _DEBUG

}


void BulletManager::SpawnBullet(int laneNumber, float time)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize();
	newBullet->SetPosition(
		{ startTranslate_.x + startDistance_ * (laneNumber - 1), startTranslate_.y, startTranslate_.z },
		{ endTranslate_.x + endDistance_ * (laneNumber - 1), endTranslate_.y, endTranslate_.z }
	);
	newBullet->SetScale(startScale_, endScale_);
	newBullet->SetTravelTime(time);
	newBullet->SetNowLine(laneNumber);
	bullets_.push_back(std::move(newBullet));
}

