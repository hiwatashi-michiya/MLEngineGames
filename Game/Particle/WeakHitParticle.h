#pragma once


#include <list>
#include <numbers>

#include "Sprite3D.h"
#include "Particle3D.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"

class WeakHitParticle
{
	struct Water {
		std::unique_ptr<MLEngine::Resource::Sprite3D> sprite3D_;
		MLEngine::Math::Vector3 maxScale_{};
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

	struct Star {
		std::unique_ptr<MLEngine::Resource::Sprite3D> sprite3D_;
		MLEngine::Math::Vector3 maxScale_{};
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
	// 水
	std::list<Water> waters_;
	// 分割数
	int waterDivision_ = 50;
	// 最大サイズ
	MLEngine::Math::Vector3 waterMaxScale_ = {0.3f, 0.3f, 0.3f};
	// 最大スピード
	MLEngine::Math::Vector3 waterMaxSpeed_ = { 0.1f, 0.3f, 0.0f };
	// オフセット
	MLEngine::Math::Vector3 waterOffset_ = { 0.0f, 0.0f, 0.0f };
	// 生存時間
	float waterLimitTime_ = 0.5f;
	// 重力
	float gravity_;

	// 星
	std::list<Star> stars_;
	// 分割数
	int starDivision_ = 20;
	// 最大サイズ
	MLEngine::Math::Vector3 starMaxScale_ = { 0.5f, 0.5f, 0.5f };
	MLEngine::Math::Vector3 starOffset_ = { 1.0f, 1.0f, 0.0f };
	// 生存時間
	float starLimitTime_ = 0.5f;

};