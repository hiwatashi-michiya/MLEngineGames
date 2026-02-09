#include "HitParticle.h"

#include <format>

#include "Math/Rand.h"
#include"Externals/imgui/imgui.h"

void HitParticle::Initialize() {
	global_ = GlobalVariables::GetInstance();

	global_->AddItem("HitParticle", "GrainDivision", 10);
	global_->AddItem("HitParticle", "GrainScale", MLEngine::Math::Vector3(0.1f, 0.1f, 0.1f));
	global_->AddItem("HitParticle", "GrainOffset", 0.1f);
	global_->AddItem("HitParticle", "GrainLimitTime", 1.0f);
	global_->AddItem("HitParticle", "GrainGravity", -0.002f);
	global_->AddItem("HitParticle", "GrainEnemy1Color", MLEngine::Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	global_->AddItem("HitParticle", "GrainEnemy2Color", MLEngine::Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	global_->AddItem("HitParticle", "SpickDivision", 10);
	global_->AddItem("HitParticle", "SpickMaxScale", MLEngine::Math::Vector3(0.1f, 1.0f, 0.0f));
	global_->AddItem("HitParticle", "SpickOffset", 0.15f);
	global_->AddItem("HitParticle", "SpickLimitTime", 0.75f);
	global_->AddItem("HitParticle", "SpickEnemy1Color", MLEngine::Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	global_->AddItem("HitParticle", "SpickEnemy2Color", MLEngine::Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	grainDivision_ = global_->GetIntValue("HitParticle", "GrainDivision");
	grainScale_ = global_->GetVector3Value("HitParticle", "GrainScale");
	grainOffset_ = global_->GetFloatValue("HitParticle", "GrainOffset");
	grainLimitTime_ = global_->GetFloatValue("HitParticle", "GrainLimitTime");
	grainGravity_ = global_->GetFloatValue("HitParticle", "GrainGravity");
	grainEnemy1Color_ = global_->GetVector4Value("HitParticle", "GrainEnemy1Color");
	grainEnemy2Color_ = global_->GetVector4Value("HitParticle", "GrainEnemy1Color");

	spickDivision_ = global_->GetIntValue("HitParticle", "SpickDivision");
	spickMaxScale_ = global_->GetVector3Value("HitParticle", "SpickMaxScale");
	spickOffset_ = global_->GetFloatValue("HitParticle", "SpickOffset");
	spickLimitTime_ = global_->GetFloatValue("HitParticle", "SpickLimitTime");
	spickEnemy1Color_ = global_->GetVector4Value("HitParticle", "SpickEnemy1Color");
	spickEnemy2Color_ = global_->GetVector4Value("HitParticle", "SpickEnemy2Color");


	grains_.clear();
	spicks_.clear();


}

void HitParticle::Update() {
	// 粒
	for (auto& grain : grains_) {
		grain.lifeTime_ += 1.0f / 60.0f;

		//for (int i = 0; i < grainDivision_; i++) {
		//	// 速度加算
		//	grain.particle3D->particleData[i].transform.translate += grain.particle3D->particleData[i].velocity;
		//	// 角度を移動方向に向けいる
		//	float angle = std::atan2f(grain.particle3D->particleData[i].velocity.y, grain.particle3D->particleData[i].velocity.x);
		//	grain.particle3D->particleData[i].transform.rotateQuaternion = MLEngine::Math::MakeRotateAxisAngleQuaternion(MLEngine::Math::Vector3::AxisZ(), angle + float(std::numbers::pi) / 2.0f);
		//	// 速度に重力適用
		//	grain.particle3D->particleData[i].velocity.y += grain.gravity_;
		//	// イージング
		//	float t = std::clamp(grain.lifeTime_ / grain.limitTime_, 0.0f, 1.0f);
		//	// 線形補間
		//	float alpha = 1.0f - 1.0f * t;
		//	// 透明化
		//	grain.particle3D->particleData[i].color.w = alpha;
		//}

		// 速度加算
		grain.sprite3D_->transform.translate += grain.velocity_;
		// 角度を移動方向に向けいる
		float angle = std::atan2f(grain.velocity_.y, grain.velocity_.x);
		grain.sprite3D_->transform.rotateQuaternion = MLEngine::Math::MakeRotateAxisAngleQuaternion(MLEngine::Math::Vector3::AxisZ(), angle + float(std::numbers::pi) / 2.0f);
		// 速度に重力適用
		grain.velocity_.y += grain.gravity_;
		// イージング
		float t = std::clamp(grain.lifeTime_ / grain.limitTime_, 0.0f, 1.0f);
		// 線形補間
		float alpha = 1.0f - 1.0f * t;
		// 透明化
		grain.sprite3D_->color.w = alpha;



	}
	// とげ
	for (auto& spick : spicks_) {
		spick.lifeTime_ += 1.0f / 60.0f;

		//for (int i = 0; i < spickDivision_; i++) {
		//	// イージング
		//	float t = std::clamp(spick.lifeTime_ / spick.limitTime_, 0.0f, 1.0f);
		//	// 山なりの波形
		//	t = std::sinf(float(std::numbers::pi) * t);
		//	// 大きく->小さく
		//	spick.particle3D->particleData[i].transform.scale = MLEngine::Math::Vector3(spick.maxScale_.x * t, spick.maxScale_.y * t, 0.0f);

		//}

		// イージング
		float t = std::clamp(spick.lifeTime_ / spick.limitTime_, 0.0f, 1.0f);
		// 山なりの波形
		t = std::sinf(float(std::numbers::pi) * t);
		// 大きく->小さく
		spick.sprite3D_->transform.scale = MLEngine::Math::Vector3(spick.maxScale_.x * t, spick.maxScale_.y * t, 0.0f);
	}

	grains_.remove_if([this](const Grain& grain) {
		return grain.IsDead();
		});

	spicks_.remove_if([this](const Spick& spick) {
		return spick.IsDead();
		});

	DebugUI();

}

void HitParticle::DebugUI() {
#ifdef _DEBUG

	ImGui::Begin("ヒットパーティクル");


	ImGui::SliderInt("粒の数", &grainDivision_, 0, 100);
	global_->datas_["HitParticle"].items["GrainDivision"].value = grainDivision_;

	ImGui::SliderFloat3("粒の大きさ", &grainScale_.x, 0.0f, 5.0f);
	global_->datas_["HitParticle"].items["GrainScale"].value = grainScale_;

	ImGui::SliderFloat("粒のオフセット", &grainOffset_, 0.0f, 1.0f);
	global_->datas_["HitParticle"].items["GrainOffset"].value = grainOffset_;

	ImGui::SliderFloat("粒の生存時間", &grainLimitTime_, 0.0f, 10.0f);
	global_->datas_["HitParticle"].items["GrainLimitTime"].value = grainLimitTime_;

	ImGui::DragFloat("粒の重力", &grainGravity_, 0.001f);
	global_->datas_["HitParticle"].items["GrainGravity"].value = grainGravity_;

#ifdef CLIENT_BUILD
	ImGui::SliderFloat4("粒の色(敵2)", &grainEnemy2Color_.x, 0.0f, 1.0f);
	global_->datas_["HitParticle"].items["GrainEnemy2Color"].value = grainEnemy2Color_;
#else
	ImGui::SliderFloat4("粒の色(敵1)", &grainEnemy1Color_.x, 0.0f, 1.0f);
	global_->datas_["HitParticle"].items["GrainEnemy1Color"].value = grainEnemy1Color_;
#endif

	ImGui::Separator();

	ImGui::SliderInt("とげの数", &spickDivision_, 0, 100);
	global_->datas_["HitParticle"].items["SpickDivision"].value = spickDivision_;

	ImGui::SliderFloat3("とげの最大", &spickMaxScale_.x, 0.0f, 5.0f);
	global_->datas_["HitParticle"].items["SpickMaxScale"].value = spickMaxScale_;

	ImGui::SliderFloat("とげのオフセット", &spickOffset_, 0.0f, 1.0f);
	global_->datas_["HitParticle"].items["SpickOffset"].value = spickOffset_;

	ImGui::SliderFloat("とげの生存時間", &spickLimitTime_, 0.0f, 10.0f);
	global_->datas_["HitParticle"].items["SpickLimitTime"].value = spickLimitTime_;


#ifdef CLIENT_BUILD
	ImGui::SliderFloat4("とげの色(敵2)", &spickEnemy2Color_.x, 0.0f, 1.0f);
	global_->datas_["HitParticle"].items["SpickEnemy2Color"].value = spickEnemy2Color_;
#else
	ImGui::SliderFloat4("とげの色(敵1)", &spickEnemy1Color_.x, 0.0f, 1.0f);
	global_->datas_["HitParticle"].items["SpickEnemy1Color"].value = spickEnemy1Color_;
#endif




	if (ImGui::Button("Save")) {
		global_->SaveFile("HitParticle");
		std::string message = std::format("{}.json saved", "HitParticle");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}
	ImGui::End();


#endif // _DEBUG
}

void HitParticle::Spawn(MLEngine::Math::Vector3 position)
{
	/*Grain newGrain;
	newGrain.particle3D.reset(MLEngine::Resource::Particle3D::Create("./Resources/EngineResources/plane/plane.obj", grainDivision_));
	newGrain.particle3D->SetTexture("./Resources/Texture/triangle.png");*/
	for (int i = 0; i < grainDivision_; i++) {
		////ビルボードフラグ
		//newGrain.particle3D->isBillboard_ = false;
		////モデル一つ一つのアクティブフラグ
		//newGrain.particle3D->particleData[i].isActive = true;
		//// スケール
		//newGrain.particle3D->particleData[i].transform.scale = { 0.1f, 0.1f, 0.1f };
		//// 位置
		//newGrain.particle3D->particleData[i].transform.translate = position;
		//// 速度
		//float angle = (2.0f * float(std::numbers::pi)) * ((float)i / (float)grainDivision_);
		//float offset = MLEngine::Math::RandomFloat(-0.1f, 0.1f);
		//newGrain.particle3D->particleData[i].velocity = MLEngine::Math::Vector3(std::cosf(angle + offset) * 0.1f, std::sinf(angle + offset) * 0.1f, 0.0f);

		Grain newGrain;
		newGrain.sprite3D_ = std::make_unique<MLEngine::Resource::Sprite3D>();
		newGrain.sprite3D_->Initialize("./Resources/Texture/triangle.png", 1);
		//モデル一つ一つのアクティブフラグ
		newGrain.sprite3D_->isActive = true;
		// スケール
		newGrain.sprite3D_->transform.scale = { 0.1f, 0.1f, 0.1f };
		// 位置
		newGrain.sprite3D_->transform.translate = position;
		// 速度
		float angle = (2.0f * float(std::numbers::pi)) * ((float)i / (float)grainDivision_);
		float offset = MLEngine::Math::RandomFloat(-0.1f, 0.1f);
		newGrain.velocity_ = MLEngine::Math::Vector3(std::cosf(angle + offset) * 0.1f, std::sinf(angle + offset) * 0.1f, 0.0f);

		

			// カラー
#ifdef CLIENT_BUILD
		//newGrain.particle3D->particleData[i].color = grainEnemy2Color_;
		newGrain.sprite3D_->color = grainEnemy2Color_;
#else
		//newGrain.particle3D->particleData[i].color = grainEnemy1Color_;
		newGrain.sprite3D_->color = grainEnemy1Color_;
#endif

		newGrain.lifeTime_ = 0.0f;
		newGrain.limitTime_ = 1.0f;
		newGrain.gravity_ = -0.002f;

		grains_.emplace_back(std::move(newGrain));

	}
	/*newGrain.lifeTime_ = 0.0f;
	newGrain.limitTime_ = 1.0f;
	newGrain.gravity_ = -0.002f;*/

	//grains_.emplace_back(std::move(newGrain));


	/*Spick newSpick;
	newSpick.particle3D.reset(MLEngine::Resource::Particle3D::Create("./Resources/EngineResources/plane/plane.obj", grainDivision_));
	newSpick.particle3D->SetTexture("./Resources/Texture/triangle.png");*/
	for (int i = 0; i < spickDivision_; i++) {
		////ビルボードフラグ
		//newSpick.particle3D->isBillboard_ = false;
		////モデル一つ一つのアクティブフラグ
		//newSpick.particle3D->particleData[i].isActive = true;
		//// スケール
		//newSpick.particle3D->particleData[i].transform.scale = { 0.0f, 0.0f, 0.0f };
		//// 位置
		//newSpick.particle3D->particleData[i].transform.translate = position;
		//// 角度
		//float angle = (2.0f * float(std::numbers::pi)) * ((float)i / (float)spickDivision_);
		//float offset = MLEngine::Math::RandomFloat(-0.15f, 0.15f);
		//newSpick.particle3D->particleData[i].transform.rotateQuaternion = MLEngine::Math::MakeRotateAxisAngleQuaternion(MLEngine::Math::Vector3::AxisZ(), angle /*+ offset*/);

		Spick newSpick;
		newSpick.sprite3D_ = std::make_unique<MLEngine::Resource::Sprite3D>();
		newSpick.sprite3D_->Initialize("./Resources/Texture/triangle.png", 1);
		//モデル一つ一つのアクティブフラグ
		newSpick.sprite3D_->isActive = true;
		// スケール
		newSpick.sprite3D_->transform.scale = { 0.0f, 0.0f, 0.0f };
		// 位置
		newSpick.sprite3D_->transform.translate = position;
		// 角度
		float angle = (2.0f * float(std::numbers::pi)) * ((float)i / (float)spickDivision_);
		float offset = MLEngine::Math::RandomFloat(-0.15f, 0.15f);
		newSpick.sprite3D_->transform.rotateQuaternion = MLEngine::Math::MakeRotateAxisAngleQuaternion(MLEngine::Math::Vector3::AxisZ(), angle /*+ offset*/);


		// カラー
#ifdef CLIENT_BUILD
		//newSpick.particle3D->particleData[i].color = spickEnemy2Color_;
		newSpick.sprite3D_->color = spickEnemy2Color_;
#else
		//newSpick.particle3D->particleData[i].color = spickEnemy1Color_;
		newSpick.sprite3D_->color = spickEnemy1Color_;
#endif

		newSpick.maxScale_ = MLEngine::Math::Vector3(0.1f, 1.0f, 0.0f);
		newSpick.lifeTime_ = 0.0f;
		newSpick.limitTime_ = 0.75f;


		spicks_.emplace_back(std::move(newSpick));

	}
	/*newSpick.maxScale_ = MLEngine::Math::Vector3(0.1f, 1.0f, 0.0f);
	newSpick.lifeTime_ = 0.0f;
	newSpick.limitTime_ = 0.75f;


	spicks_.emplace_back(std::move(newSpick));*/


}
