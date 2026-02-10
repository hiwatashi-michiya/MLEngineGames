#include "SmokeParticle.h"

#include <format>

#include "Math/Rand.h"
#include"Externals/imgui/imgui.h"

void SmokeParticle::Initialize() {
	smokes_.clear();

}

void SmokeParticle::Update() {
	for (auto& smoke : smokes_) {
		smoke.lifeTime_ += 1.0f / 60.0f;

		smoke.sprite3D_->transform.translate += smoke.velocity_;
		smoke.velocity_.y += smoke.gravity_;

		float t = std::clamp( smoke.lifeTime_ / smoke.limitTime_, 0.0f, 1.0f);
		float sway = std::sinf(t * 2.0f + MLEngine::Math::RandomFloat(-3.0f, 3.0f)) * 0.0015f;
		smoke.sprite3D_->transform.translate.x += sway;

		float scale = 0.2f + 0.8f * t;
		smoke.sprite3D_->transform.scale = { scale, scale, 1.0f };
		smoke.sprite3D_->color.w = std::sinf(float(std::numbers::pi) * t);
	}
}

void SmokeParticle::DebugUI() {

}

void SmokeParticle::Spawn(MLEngine::Math::Vector3 position) {
	for (int i = 0; i < 1; i++) {
		Smoke newSmoke;
		newSmoke.sprite3D_ = std::make_unique<MLEngine::Resource::Sprite3D>();
		newSmoke.sprite3D_->Initialize("./Resources/Texture/smoke.png", 1);
		newSmoke.sprite3D_->isActive = true;
		newSmoke.sprite3D_->transform.scale = { 0.0f, 0.0f, 1.0f };
		newSmoke.sprite3D_->transform.translate = position;
		float angle = MLEngine::Math::RandomFloat(-0.5f, 0.5f);
		float speed = MLEngine::Math::RandomFloat(0.01f, 0.03f);
		newSmoke.velocity_ = { std::sinf(angle) * speed, speed * 1.5f, 0.0f };
		newSmoke.gravity_ = -0.0002f;

		newSmoke.lifeTime_ = 0.0f;
		newSmoke.limitTime_ = 0.75f;

		smokes_.emplace_back(std::move(newSmoke));
	}
	
}