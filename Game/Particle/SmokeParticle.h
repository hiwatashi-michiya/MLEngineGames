#pragma once

#include <list>
#include <numbers>

#include "Sprite3D.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"


class SmokeParticle {
	struct Smoke {
		
		std::unique_ptr <MLEngine::Resource::Sprite3D> sprite3D_;
		MLEngine::Math::Vector3 velocity_;
		float gravity_ = 0.01f;
		float lifeTime_ = 0.0f;
		float limitTime_ = 0.0f;
		// 死亡フラグ
		bool IsDead() const {
			if (std::clamp(lifeTime_ / limitTime_, 0.0f, 1.0f) == 1.0f) {
				return true;
			}
			return false;
		}
	};

public:
	void Initialize();
	void Update();
	void DebugUI();

	void Spawn(MLEngine::Math::Vector3 position);
private:
	GlobalVariables* global_;

	// 粒
	std::list<Smoke> smokes_;
	// 数
	int smokeVolume = 5;
	// スケール
	MLEngine::Math::Vector3 smokeScale_;
	// オフセット
	float offset_;
	// 生存時間
	float grainLimitTime_;
	// 重力
	float gravity_;

};