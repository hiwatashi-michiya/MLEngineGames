#pragma once

#include "Sprite3D.h"
#include "Object/Transform.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"

class EnemyHand {
public:
	enum class HandState {
		kNone,
		kNormal,
		kAngry,
		kAttack
	};

	struct EffectTime {
		float elapsedTime_;
		float targetTime_;
	};

public:
	EnemyHand() {};
	~EnemyHand() {};

	void Initialize(MLEngine::Object::Transform* parentTransform, bool isLeft);
	void Update();
	void DebugUI(std::string uiname, std::string dir);

	HandState GetPreviousHandState() { return prevHandState_; }
	HandState GetHandState() { return handState_; }
	void SetHandState(HandState state);
	void SetIsActive(const bool isActive) {
		frontPlane_.isActive = isActive;
		backPlane_.isActive = isActive;
	}

private:

	std::string enemytype_;

	bool isLeft_ = true;

	GlobalVariables* global_;

	// 3Dスプライト
	MLEngine::Resource::Sprite3D frontPlane_;
	MLEngine::Resource::Sprite3D backPlane_;
	// 接続部分スプライト
	MLEngine::Resource::Sprite3D joint_;

	// テクスチャパス
	std::string frontTexture_;
	std::string backTexture_;
	std::string frontFistTexture_;
	std::string backFistTexture_;

	// 手の状態
	HandState handState_ = HandState::kNormal;
	HandState prevHandState_ = HandState::kNormal;

	MLEngine::Math::Vector3 armAngleAxis_ = { 0.0f, 0.0f, 0.0f };

	// 接続用のトランスフォーム
	std::unique_ptr<MLEngine::Object::Transform> jointTransform_;
	std::unique_ptr<MLEngine::Object::Transform> transform_;

	MLEngine::Math::Vector3 startPosition_;

	// 腕振り用の振り幅と速度
	float normalSwingAngle_ = 30.0f;
	float angrySwingAngle_ = 60.0f;
	float swingSpeed_ = 0.0f;
	float angle_ = 0.0f;

	// 攻撃用パラメーター
	EffectTime swingUpTime_{ 0.0f, 0.2f };
	EffectTime swingDownTime_{ 0.0f, 0.3f };
	EffectTime afterTime_{ 0.0f, 0.5f };
	float attackDuration_ = 0.5f;
	float amplitude_ = 0.0f;
	float swingUp_ = 0.0f;
	float swingDown_ = 0.0f;

	float normalAttackTime_ = 1.0f;
	float angryAttackTime_ = 0.5f;
	float attackMotionTime_ = 0.5f;
	float totalTime_;

};