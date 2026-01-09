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

public:
	EnemyHand() {};
	~EnemyHand() {};

	void Initialize(MLEngine::Object::Transform* parentTransform, bool isLeft);
	void Update();
	void DebugUI(std::string uiname, std::string dir);

	void SetHandState(HandState state) { handState_ = state; }

	void SetParentTransform(MLEngine::Object::Transform* parentTransform) {
		jointTransform_->SetParent(parentTransform);
	}

private:

	bool isLeft_ = true;

	GlobalVariables* global_;

	// 3Dスプライト
	MLEngine::Resource::Sprite3D frontPlane_;
	MLEngine::Resource::Sprite3D backPlane_;

	MLEngine::Resource::Sprite3D joint_;

	// テクスチャパス
	std::string frontTexture_;
	std::string backTexture_;
	std::string frontFistTexture_;
	std::string backFistTexture_;

	// 手の状態
	HandState handState_ = HandState::kNormal;

	// 接続用のトランスフォーム
	std::unique_ptr<MLEngine::Object::Transform> jointTransform_;
	std::unique_ptr<MLEngine::Object::Transform> transform_;

	float swingAngle_ = 30.0f;
	float swingSpeed_ = 0.0f;
	float angleSpeed_ = 0.0f;
	float angle_ = 0.0f;

};