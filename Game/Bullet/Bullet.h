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

	// 跳ね返り処理
	void Reverse() {
		//velocity_.x = -velocity_.x; velocity_.y = -velocity_.y;

	}
	// 死亡判定
	bool IsDead() const { return !isActive_; }
	//// 位置セット
	//void SetPosition(const MLEngine::Math::Vector2& position) {
	//	position_ = position;
	//	startPosition_ = position;
	//	sprite_->SetPosition(position_);
	//}
	//// 速度セット
	//void SetVelocity(MLEngine::Math::Vector2 direction, float speed) {
	//	velocity_ = { direction.x * speed, direction.y * speed };
	//}
	//// 目標位置セット
	//void SetTargetPosition(MLEngine::Math::Vector2 targetPosition) { targetPosition_ = targetPosition; }
	//// サイズ(最小・最大)セット
	//void SetSize(float minSize, float maxSize) { minSize_ = minSize; maxSize_ = maxSize; }
	//// 終点ラインセット
	//void SetEndLine(float endLine) { endLine_ = endLine; }

	void SetPosition(const MLEngine::Math::Vector3& startPosition, const MLEngine::Math::Vector3& targetPosition) {
		position_ = startPosition;
		startPosition_ = position_;
		targetPosition_ = targetPosition;
		model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, position_);
	}
	void SetTargetPosition(const MLEngine::Math::Vector3& targetPosition) {
		//targetPosition_ = targetPosition;
	}
	void SetScale(const MLEngine::Math::Vector3& startScale, const MLEngine::Math::Vector3& endScale) {
		scale_ = startScale;
		startScale_ = scale_;
		endScale_ = endScale;
		model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, scale_);
	}


	// 移動時間セット
	void SetTravelTime(float time) { travelTime_ = time; elapsedTime_ = 0.0f; }

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


	//// スプライト
	//std::unique_ptr<MLEngine::Resource::Sprite> sprite_;
	//// Vector2宣言
	//MLEngine::Math::Vector2 position_;
	//// 移動速度
	//MLEngine::Math::Vector2 velocity_ = { 0.0f, 1.0f };
	//// 最小サイズ
	//float minSize_ = 32.0f;
	//// 最大サイズ
	//float maxSize_ = 128.0f;
	//// サイズ倍率
	//const float multiplier_ = 4.0f;
	//// 終点ライン
	//float endLine_ = 640.0f;

	//MLEngine::Math::Vector2 startPosition_;
	//// 目標位置
	//MLEngine::Math::Vector2 targetPosition_;
	// 移動時間
	float travelTime_ = 0.0f;
	// 経過時間
	float elapsedTime_ = 0.0f;
};

