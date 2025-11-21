#pragma once

#include "Object/GameObject.h"
#include "Object/Transform.h"
#include "Math/Rand.h"
#include "EnemyState.h"
#include "../Bullet/BulletManager.h"
#include "RigidModel.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"

class Enemy :public MLEngine::Object::GameObject
{
public:
	const char* states[3] = {
	"Normal",
	"Down",
	"Berserk"
	};

public:
	Enemy() {};
	~Enemy() {};
	void Initialize() override;
	void Update() override;
	void Draw(MLEngine::Object::Camera* camera) override;
	void ImguiDraw();

	// 状態変更
	void ChangeState(std::unique_ptr<EnemyState> newState);
	// 発射位置
	//MLEngine::Math::Vector2 LaunchPosition();

	BulletManager* GetBulletManger() { return bulletManager_.get(); }
	//float GetDistance() const { return distance_; }

private:
	GlobalVariables* global_;

	std::unique_ptr<EnemyState> currentState_;
	MLEngine::Resource::RigidModel model_;
	MLEngine::Math::Vector3 scale_ = { 2.0f, 1.0f, 1.0f };
	MLEngine::Math::Vector3 translate_ = { 0.0f, 1.0f,0.0f };

	// 弾マネージャー
	std::unique_ptr<BulletManager> bulletManager_;

	//// スプライト
	std::unique_ptr<MLEngine::Resource::Sprite> sprite_;
	//// 位置
	//MLEngine::Math::Vector2 position_;
	//// 初期位置
	//const MLEngine::Math::Vector2 initialPosition_ = { 640.0f, 100.0f };
	//const float distance_ = 100.0f;

	// 体力
	int maxHp_ = 500;
	int hp_ = 0;

	// ImGui用状態選択インデックス
	int stateIndex = 0;

	
};

