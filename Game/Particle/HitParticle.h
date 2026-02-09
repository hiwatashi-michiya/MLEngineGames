#pragma once

#include <list>
#include <numbers>

#include "Sprite3D.h"
#include "Particle3D.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"



class HitParticle {
	struct Grain {
		//std::unique_ptr<MLEngine::Resource::Particle3D> particle3D;
		std::unique_ptr < MLEngine::Resource::Sprite3D> sprite3D_;
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

	struct Spick {
		//std::unique_ptr<MLEngine::Resource::Particle3D> particle3D;
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

	// 粒
	std::list<Grain> grains_;
	// 分割数
	int grainDivision_;
	// スケール
	MLEngine::Math::Vector3 grainScale_;
	// オフセット
	float grainOffset_;
	// 生存時間
	float grainLimitTime_;
	// 重力
	float grainGravity_;
	// 敵1カラー
	MLEngine::Math::Vector4 grainEnemy1Color_;
	// 敵2カラー
	MLEngine::Math::Vector4 grainEnemy2Color_;

	// とげ
	std::list<Spick> spicks_;
	// 分割数
	int spickDivision_;
	// 最大サイズ
	MLEngine::Math::Vector3 spickMaxScale_;
	// オフセット
	float spickOffset_;
	// 生存時間
	float spickLimitTime_;
	// 敵1カラー
	MLEngine::Math::Vector4 spickEnemy1Color_;
	// 敵2カラー
	MLEngine::Math::Vector4 spickEnemy2Color_;
	
	

};