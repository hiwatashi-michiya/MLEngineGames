#include "WeakHitParticle.h"

#include <format>

#include "Math/Rand.h"
#include"Externals/imgui/imgui.h"

void WeakHitParticle::Initialize() {
	global_ = GlobalVariables::GetInstance();

	global_->AddItem("WeakHitParticle", "WaterDivision", 10);
	global_->AddItem("WeakHitParticle", "WaterMaxScale", MLEngine::Math::Vector3(0.1f, 0.1f, 0.1f));
	global_->AddItem("WeakHitParticle", "WaterMaxSpeed", MLEngine::Math::Vector3(0.1f, 0.3f, 0.0f));
	global_->AddItem("WeakHitParticle", "WaterOffset", MLEngine::Math::Vector3(0.0f, 0.0f, 0.0f));
	global_->AddItem("WeakHitParticle", "WaterLimitTime", 1.0f);
	global_->AddItem("WeakHitParticle", "WaterGravity", -0.002f);

	global_->AddItem("WeakHitParticle", "StarDivision", 20);
	global_->AddItem("WeakHitParticle", "StarMaxScale", MLEngine::Math::Vector3(0.5f, 0.5f, 0.5f));
	global_->AddItem("WeakHitParticle", "StarOffset", MLEngine::Math::Vector3(1.0f, 1.0f, 0.0f));
	global_->AddItem("WeakHitParticle", "StarLimitTime", 0.5f);
	

	waterDivision_ = global_->GetIntValue("WeakHitParticle", "WaterDivision");
	waterMaxScale_ = global_->GetVector3Value("WeakHitParticle", "WaterMaxScale");
	waterMaxSpeed_ = global_->GetVector3Value("WeakHitParticle", "WaterMaxSpeed");
	waterOffset_ = global_->GetVector3Value("WeakHitParticle", "WaterOffset");
	waterLimitTime_ = global_->GetFloatValue("WeakHitParticle", "WaterLimitTime");
	gravity_ = global_->GetFloatValue("WeakHitParticle", "WaterGravity");

	starDivision_ = global_->GetIntValue("WeakHitParticle", "StarDivision");
	starMaxScale_ = global_->GetVector3Value("WeakHitParticle", "StarMaxScale");
	starOffset_ = global_->GetVector3Value("WeakHitParticle", "StarOffset");
	starLimitTime_ = global_->GetFloatValue("WeakHitParticle", "StarLimitTime");


	waters_.clear();
	stars_.clear();
}

void WeakHitParticle::Update() {
	for(auto& water : waters_){
		water.lifeTime_ += 1.0f / 60.0f;
		water.sprite3D_->transform.translate += water.velocity_;
		water.velocity_.y += water.gravity_;
		float t = std::clamp(water.lifeTime_ / water.limitTime_, 0.0f, 1.0f);
		water.sprite3D_->transform.scale = MLEngine::Math::Vector3(water.maxScale_.x - t * waterMaxScale_.x, water.maxScale_.y - t * waterMaxScale_.y, 0.0f);
	}

	for(auto& star : stars_){
		star.lifeTime_ += 1.0f / 60.0f;
		// イージング
		float t = std::clamp(star.lifeTime_ / star.limitTime_, 0.0f, 1.0f);
		// 山なりの波形
		t = std::sinf(float(std::numbers::pi) * t);
		// 大きく->小さく
		star.sprite3D_->transform.scale = MLEngine::Math::Vector3(star.maxScale_.x * t, star.maxScale_.y * t, 0.0f);
	}

	waters_.remove_if([this](const Water& water) {
		return water.IsDead();
	});

	stars_.remove_if([this](const Star& star) {
		return star.IsDead();
	});

	DebugUI();
}

void WeakHitParticle::DebugUI() {
#ifdef _DEBUG

	ImGui::Begin("水パーティクル");


	ImGui::SliderInt("水の数", &waterDivision_, 0, 100);
	global_->datas_["WeakHitParticle"].items["WaterDivision"].value = waterDivision_;

	ImGui::SliderFloat3("水の最大サイズ", &waterMaxScale_.x, 0.0f, 1.0f);
	global_->datas_["WeakHitParticle"].items["WaterMaxScale"].value = waterMaxScale_;

	ImGui::SliderFloat3("水の最大スピード", &waterMaxSpeed_.x, 0.0f, 1.0f);
	global_->datas_["WeakHitParticle"].items["WaterMaxSpeed"].value = waterMaxSpeed_;

	ImGui::DragFloat3("水のオフセット", &waterOffset_.x, 0.1f);
	global_->datas_["WeakHitParticle"].items["WaterOffset"].value = waterOffset_;

	ImGui::SliderFloat("水の生存時間", &waterLimitTime_, 0.0f, 10.0f);
	global_->datas_["WeakHitParticle"].items["WaterLimitTime"].value = waterLimitTime_;

	ImGui::DragFloat("水の重力", &gravity_, 0.001f);
	global_->datas_["WeakHitParticle"].items["WaterGravity"].value = gravity_;

	ImGui::Separator();

	ImGui::SliderInt("星の数", &starDivision_, 0, 100);
	global_->datas_["WeakHitParticle"].items["StarDivision"].value = starDivision_;

	ImGui::SliderFloat3("星の最大サイズ", &starMaxScale_.x, 0.0f, 1.0f);
	global_->datas_["WeakHitParticle"].items["StarMaxScale"].value = starMaxScale_;

	ImGui::DragFloat3("星のオフセット", &starOffset_.x, 0.1f);
	global_->datas_["WeakHitParticle"].items["StarOffset"].value = starOffset_;

	ImGui::SliderFloat("星の生存時間", &starLimitTime_, 0.0f, 10.0f);
	global_->datas_["WeakHitParticle"].items["StarLimitTime"].value = starLimitTime_;


	if (ImGui::Button("Save")) {
		global_->SaveFile("WeakHitParticle");
		std::string message = std::format("{}.json saved", "WeakHitParticle");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}
	ImGui::End();


#endif // _DEBUG
}

void WeakHitParticle::Spawn(MLEngine::Math::Vector3 position, bool waterOnly){

	for (int i = 0; i < waterDivision_; i++) {
		Water newWater;
		newWater.sprite3D_ = std::make_unique<MLEngine::Resource::Sprite3D>();
		newWater.sprite3D_->Initialize("./Resources/Texture/water.png", 1);
		newWater.sprite3D_->isActive = true;
		newWater.sprite3D_->transform.scale = waterMaxScale_;
		newWater.sprite3D_->transform.translate = position;
		float angle = (float(std::numbers::pi)) * ((float)i / (float)waterDivision_);
		//float offset = MLEngine::Math::RandomFloat(-0.1f, 0.1f);
		newWater.velocity_ = MLEngine::Math::RandomVector3(-waterOffset_, waterOffset_) + MLEngine::Math::Vector3((std::cosf(angle)) * waterMaxSpeed_.x, (std::sinf(angle)) * waterMaxSpeed_.y, 0.0f);
		//newWater.sprite3D_->color = { 0.0f, 0.0f, 1.0f, 1.0f };

		newWater.maxScale_ = waterMaxScale_;
		newWater.lifeTime_ = 0.0f;
		newWater.limitTime_ = waterLimitTime_;
		newWater.gravity_ = -0.01f;
		newWater.sprite3D_->transform.UpdateMatrix();
		waters_.emplace_back(std::move(newWater));
	}

	if (waterOnly) {
		return;
	}
	for(int i = 0; i < starDivision_; i++){
		Star newStar;
		newStar.sprite3D_ = std::make_unique<MLEngine::Resource::Sprite3D>();
		newStar.sprite3D_->Initialize("./Resources/Texture/triangle.png", 1);
		newStar.sprite3D_->isActive = true;
		newStar.sprite3D_->transform.scale = {0.0f, 0.0f, 0.0f};
		MLEngine::Math::Vector3 offset = { MLEngine::Math::RandomFloat(-starOffset_.x, starOffset_.x), MLEngine::Math::RandomFloat(0.0f, starOffset_.y), 0.0f};
		newStar.sprite3D_->transform.translate = position + MLEngine::Math::Vector3(0.0f, 0.0f, -3.0f) + offset;
		newStar.sprite3D_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		newStar.maxScale_ = starMaxScale_;
		newStar.lifeTime_ = 0.0f;
		newStar.limitTime_ = starLimitTime_;
		newStar.sprite3D_->transform.UpdateMatrix();

		stars_.emplace_back(std::move(newStar));
	}
}
