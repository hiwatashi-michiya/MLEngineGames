#include "BulletManager.h"

#include <format>

#include "Input/Input.h"
#include"Externals/imgui/imgui.h"
#include "Enemy/Enemy.h"
#include "Math/Rand.h"

void BulletManager::Initialize(Player* player, Enemy* enemy)
{
	global_ = GlobalVariables::GetInstance();

	bullets_.clear();
	
	startScale_ = global_->GetVector3Value("BulletParameters", "StartScale");
	endScale_ = global_->GetVector3Value("BulletParameters", "EndScale");
	startTranslate_ = global_->GetVector3Value("BulletParameters", "StartTranslate");
	endTranslate_ = global_->GetVector3Value("BulletParameters", "EndTranslate");
	startDistance_ = global_->GetFloatValue("BulletParameters", "StartDistance");
	endDistance_ = global_->GetFloatValue("BulletParameters", "EndDistance");
	bulletDamege_ = global_->GetIntValue("BulletParameters", "BulletDamege");
	reflectSpeed_ = global_->GetFloatValue("BulletParameters", "ReflectSpeed");
	normalBumerator_ = global_->GetIntValue("BulletParameters", "NormalNumerator");
	normalDenominator_ = global_->GetIntValue("BulletParameters", "NormalDenominator");
	weakNumerator_ = global_->GetIntValue("BulletParameters", "WeakNumerator");
	weakDenominator_ = global_->GetIntValue("BulletParameters", "WeakDenominator");

	player_ = player;
	enemy_ = enemy;

	bulletCaveat_ = std::make_unique<BulletCaveat>();
	bulletCaveat_->Initialize(player_);

	smokeParticle_ = std::make_unique<SmokeParticle>();
	smokeParticle_->Initialize();


#ifdef _DEBUG
	startSprite3D_.clear();
	endSprite3D_.clear();

	for (int i = 0; i < 3; ++i) {
		std::unique_ptr<MLEngine::Resource::Sprite3D> startSprite3D = std::make_unique<MLEngine::Resource::Sprite3D>();
		startSprite3D->Initialize("./Resources/Texture/enemy_bullet.png", 6);
		startSprite3D->transform.scale = startScale_;
		startSprite3D->transform.translate = startTranslate_;
		startSprite3D->color = { 0.0f, 1.0f, 0.0f, 0.5f };
		startSprite3D_.push_back(std::move(startSprite3D));

		std::unique_ptr<MLEngine::Resource::Sprite3D> endSprite3D = std::make_unique<MLEngine::Resource::Sprite3D>();
		endSprite3D->Initialize("./Resources/Texture/enemy_bullet.png", 6);
		endSprite3D->transform.scale = endScale_;
		endSprite3D->transform.translate = endTranslate_;
		endSprite3D->color = { 1.0f, 0.0f, 0.0f, 0.5f };
		endSprite3D_.push_back(std::move(endSprite3D));
	}

#endif
	

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


			if(bullet->IsReflect() && bullet->GetBulletType() == Bullet::BulletType::kNormal){
				// 反射していたら敵にダメージを与える
				enemy_->OnCollision(bullet->GetPosition(), bulletDamege_);
				return true;
			}
			
			if (player_->GetNowLine() != bullet->GetNowLine()) { // レーンが違う場合は当たらない
				smokeParticle_->Spawn(bullet->GetPosition());
				return true;
			}


			if (player_->GetIsForward()) { // プレイヤーが前を向いている場合のみダメージを受ける
				
				if (bullet->GetBulletType() == Bullet::BulletType::kNormal) {
					player_->OnCollision(bulletDamege_);
				}
				else {
					player_->OnCollision(0);
				}
			}
			else { // 敵がダメージを受ける
				//enemy_->OnCollision(bulletDamege_);
				SpawnReflectBullet(bullet->GetNowLine(), reflectSpeed_, bullet->GetBulletType());
			}

			return true;
		}
		return false;
	});

	bulletCaveat_->Update();
	bulletCaveat_->DebugUI();

	smokeParticle_->Update();

#ifdef _DEBUG

	// 始点・終点モデルの更新
	for (int i = 0; i < 3; ++i) {
		startSprite3D_[i]->transform.scale = startScale_;
		startSprite3D_[i]->transform.translate = { startTranslate_.x + startDistance_ * (i - 1), startTranslate_.y, startTranslate_.z };
		endSprite3D_[i]->transform.scale = endScale_;
		endSprite3D_[i]->transform.translate = { endTranslate_.x + endDistance_ * (i - 1), endTranslate_.y, endTranslate_.z };

	}

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

	ImGui::DragFloat("反射速度", &reflectSpeed_, 0.01f);
	global_->datas_["BulletParameters"].items["ReflectSpeed"].value = reflectSpeed_;

	ImGui::Separator();

	ImGui::SliderInt("通常弾の分子", &normalBumerator_, 0, 100);
	global_->datas_["BulletParameters"].items["NormalNumerator"].value = normalBumerator_;
	ImGui::SliderInt("通常弾の分母", &normalDenominator_, 1, 100);
	global_->datas_["BulletParameters"].items["NormalDenominator"].value = normalDenominator_;
	ImGui::SliderInt("弱点弾の分子", &weakNumerator_, 0, 100);
	global_->datas_["BulletParameters"].items["WeakNumerator"].value = weakNumerator_;
	ImGui::SliderInt("弱点弾の分母", &weakDenominator_, 1, 100);
	global_->datas_["BulletParameters"].items["WeakDenominator"].value = weakDenominator_;

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
	std::string texturePath;
	Bullet::BulletType bulletType = GetBulletType(normalBumerator_, normalDenominator_, weakNumerator_, weakDenominator_);
	switch (bulletType)
	{
	case Bullet::BulletType::kNormal:
		texturePath = "./Resources/Texture/enemy_bullet.png";
		break;
	case Bullet::BulletType::kWeak:
		texturePath = "./Resources/Texture/enemy_bullet.png";
		break;
	}

	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize(texturePath, bulletType);
	newBullet->SetPosition(
		{ startTranslate_.x + startDistance_ * (laneNumber - 1), startTranslate_.y, startTranslate_.z },
		{ endTranslate_.x + endDistance_ * (laneNumber - 1), endTranslate_.y, endTranslate_.z }
	);
	newBullet->SetScale(startScale_, endScale_);
	newBullet->SetTravelTime(time);
	newBullet->SetNowLine(laneNumber);
	bullets_.push_back(std::move(newBullet));
}

void BulletManager::SpawnReflectBullet(int laneNumber, float time, Bullet::BulletType type)
{
	std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
	newBullet->Initialize("./Resources/Texture/enemy_backBullet.png", type);
	newBullet->SetPosition(
		{ endTranslate_.x + endDistance_ * (laneNumber - 1), endTranslate_.y, endTranslate_.z },
		{ startTranslate_.x + startDistance_ * (laneNumber - 1), startTranslate_.y, startTranslate_.z }
	);
	newBullet->SetScale(endScale_, startScale_);
	newBullet->SetTravelTime(time);
	newBullet->SetNowLine(laneNumber);
	newBullet->SetIsReflect(true);
	bullets_.push_back(std::move(newBullet));
}

Bullet::BulletType BulletManager::GetBulletType(int normalNum, int normalDen, int weakNum, int weakDen)
{
	if (normalDen <= 0 || weakDen <= 0)
		return Bullet::BulletType::kNormal;

	float normalRate = (float)normalNum / normalDen;
	float weakRate = (float)weakNum / weakDen;

	float total = normalRate + weakRate;

	if (total <= 0.0f)
		return Bullet::BulletType::kNormal;

	// 正規化（合計1にする）
	normalRate /= total;
	weakRate /= total;

	float r = MLEngine::Math::RandomFloat(0.0f, 1.0f);

	if (r < normalRate)
		return Bullet::BulletType::kNormal;
	else
		return Bullet::BulletType::kWeak;
}

