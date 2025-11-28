#pragma once

#include "Object/GameObject.h"
#include "Object/Transform.h"
#include "Object/Camera.h"
#include "Sprite/Sprite.h"
#include "RigidModel.h"

class Bullet : public MLEngine::Object::GameObject
{
public:
	Bullet() {};
	~Bullet() {};
	void Initialize() override;
	void Update() override;
	void Draw(MLEngine::Object::Camera* camera) override;
	// 死亡判定
	bool IsDead() const { return !isActive_; }

	// 現在のライン取得
	int GetNowLine() const { return nowLine_; }

	// 位置セット
	void SetPosition(const MLEngine::Math::Vector3& startPosition, const MLEngine::Math::Vector3& targetPosition) {
		position_ = startPosition;
		startPosition_ = position_;
		targetPosition_ = targetPosition;
		model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, position_);
	}

	// 目標位置セット
	void SetTargetPosition(const MLEngine::Math::Vector3& targetPosition) {
		//targetPosition_ = targetPosition;
	}

	// スケールセット
	void SetScale(const MLEngine::Math::Vector3& startScale, const MLEngine::Math::Vector3& endScale) {
		scale_ = startScale;
		startScale_ = scale_;
		endScale_ = endScale;
		model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, scale_);
	}

	// 移動時間セット
	void SetTravelTime(float time) { travelTime_ = time; elapsedTime_ = 0.0f; }

	// 現在のラインセット
	void SetNowLine(int line) { nowLine_ = line; }

private:
	// モデル
	MLEngine::Resource::RigidModel model_;
	// スケール
	MLEngine::Math::Vector3 scale_ = { 0.2f, 0.2f, 0.2f };
	// 平行移動
	MLEngine::Math::Vector3 position_ = { 0.0f, 0.0f,0.0f };
	// 開始位置
	MLEngine::Math::Vector3 startPosition_ = { 0.0f, 0.0f,0.0f };
	// 目標位置
	MLEngine::Math::Vector3 targetPosition_ = { 0.0f, 0.0f,0.0f };
	// 初期スケール
	MLEngine::Math::Vector3 startScale_;
	// 終了スケール
	MLEngine::Math::Vector3 endScale_;

	//アクティブ状態かどうか
	bool isActive_ = true;

	//現在いるラインの番号
	int nowLine_;

	// 移動時間
	float travelTime_ = 0.0f;
	// 経過時間
	float elapsedTime_ = 0.0f;
};

