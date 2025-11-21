#include "BulletManager.h"

#include "Input/Input.h"
#include"Externals/imgui/imgui.h"

void BulletManager::Initialize()
{
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

	for(auto& bullet : bullets_)
	{
		bullet->Update();

		/*if (input->GetKeyboard()->TriggerKey(DIK_R)) {
			bullet->Reverse();
		}*/

	}

	bullets_.remove_if([](const std::unique_ptr<Bullet>& bullet) {
		return bullet->IsDead();
	});

	for (int i = 0; i < 3; ++i) {
		/*startSprites_[i]->SetPosition({ launchPosition_.x + startDistance_ * (i - 1), launchPosition_.y });
		startSprites_[i]->size = { minSize_, minSize_ };
		targetSprites_[i]->SetPosition({ launchPosition_.x + endDistance_ * (i - 1), endLine_ });
		targetSprites_[i]->size = { maxSize_, maxSize_ };*/

		startModels_[i]->worldMatrix = MLEngine::Math::MakeAffineMatrix(startScale_, { 0.0f, 0.0f, 0.0f, 1.0f }, { startTranslate_.x + startD_ * (i - 1), startTranslate_.y, startTranslate_.z });
		endModels_[i]->worldMatrix = MLEngine::Math::MakeAffineMatrix(endScale_, { 0.0f, 0.0f, 0.0f, 1.0f }, { endTranslate_.x + endD_ * (i - 1), endTranslate_.y, endTranslate_.z });

	}

	ImGui::Begin("弾パラメーター");

	// 間隔
	/*ImGui::DragFloat("始点間隔", &startDistance_, 1.0f);
	ImGui::DragFloat("終点間隔", &endDistance_, 1.0f);*/
	ImGui::DragFloat("始点D", &startD_, 0.1f);
	ImGui::DragFloat("終点D", &endD_, 0.01f);

	ImGui::DragFloat3("始点平行移動", &startTranslate_.x, 0.1f);
	ImGui::DragFloat3("終点平行移動", &endTranslate_.x, 0.1f);

	// サイズ
	/*ImGui::DragFloat("最小サイズ", &minSize_, 1.0f);
	ImGui::DragFloat("最大サイズ", &maxSize_, 1.0f);*/

	float startSize = startScale_.x;
	ImGui::DragFloat("始点サイズ", &startSize, 0.01f);
	startScale_ = { startSize, startSize, 1.0f };

	float endSize = endScale_.x;
	ImGui::DragFloat("終点サイズ", &endSize, 0.01f);
	endScale_ = { endSize, endSize, 1.0f };

	//ImGui::DragFloat("終点ライン", &endLine_, 1.0f);

	ImGui::End();

}

void BulletManager::Draw(MLEngine::Object::Camera* camera)
{
	for(auto& bullet : bullets_)
	{
		bullet->Draw(camera);
	}
}

void BulletManager::SpawnBullet(const MLEngine::Math::Vector2& position, const MLEngine::Math::Vector2& direction, float speed)
{
	/*std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize();
	newBullet->SetPosition(position);
	newBullet->SetVelocity(direction, speed);
	bullets_.push_back(std::move(newBullet));*/


}

void BulletManager::SpawnBullet(const MLEngine::Math::Vector2& position, int laneNumber, float time)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize();
	/*newBullet->SetPosition({ position.x + startDistance_ * (laneNumber - 1), position.y });
	newBullet->SetTargetPosition({ position.x + endDistance_ * (laneNumber - 1), endLine_ });
	newBullet->SetSize(minSize_, maxSize_);
	newBullet->SetEndLine(endLine_);*/
	newBullet->SetTravelTime(time);
	bullets_.push_back(std::move(newBullet));
}

void BulletManager::SpawnBullet(int laneNumber, float time)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize();
	newBullet->SetPosition(
		{ startTranslate_.x + startD_ * (laneNumber - 1), startTranslate_.y, startTranslate_.z },
		{ endTranslate_.x + endD_ * (laneNumber - 1), endTranslate_.y, endTranslate_.z }
	);
	newBullet->SetScale(startScale_, endScale_);
	newBullet->SetTravelTime(time);
	bullets_.push_back(std::move(newBullet));
}

