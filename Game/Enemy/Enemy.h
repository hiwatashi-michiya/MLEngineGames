#pragma once

#include "Object/GameObject.h"
#include "Object/Transform.h"
#include "Math/Rand.h"
#include "EnemyState.h"
#include "../Bullet/BulletManager.h"

class Enemy :public MLEngine::Object::GameObject
{
public:
	Enemy() {};
	~Enemy() {};
	void Initialize() override;
	void Update() override;
	void Draw(MLEngine::Object::Camera* camera) override;

	// 状態変更
	void ChangeState(std::unique_ptr<EnemyState> newState);
	// 発射位置
	MLEngine::Math::Vector2 LaunchPosition();

	BulletManager* GetBulletManger() { return bulletManager_.get(); }
	float GetDistance() const { return distance_; }

private:
	std::unique_ptr<EnemyState> currentState_;

	// スプライト
	std::unique_ptr<MLEngine::Resource::Sprite> sprite_;
	// 弾マネージャー
	std::unique_ptr<BulletManager> bulletManager_;
	// 位置
	MLEngine::Math::Vector2 position_;
	// 初期位置
	const MLEngine::Math::Vector2 initialPosition_ = { 640.0f, 100.0f };
	const float distance_ = 100.0f;

	// 体力
	int maxHp_ = 500;
	int hp_ = 0;

	
};

