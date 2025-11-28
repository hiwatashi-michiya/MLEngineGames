#pragma once

#include "Object/Transform.h"
#include "Math/Rand.h"
#include "EnemyState.h"
#include "Bullet/BulletManager.h"
#include "RigidModel.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"


class Enemy
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
	void Initialize();
	void Update();

	// 状態変更
	void ChangeState(std::unique_ptr<EnemyState> newState);

	// 衝突処理
	void OnCollision(int damege);

	BulletManager* GetBulletManager() { return bulletManager_; }

	// 弾マネージャー取得
	void SetBulletManager(BulletManager* bulletManager) {
		bulletManager_ = bulletManager;
	}

private:
	GlobalVariables* global_;

	// 現在の状態
	std::unique_ptr<EnemyState> currentState_;

	// モデル
	MLEngine::Resource::RigidModel model_;

	// スケール・平行移動
	MLEngine::Math::Vector3 scale_ = { 2.0f, 1.0f, 1.0f };
	MLEngine::Math::Vector3 translate_ = { 0.0f, 1.0f,0.0f };

	// 弾マネージャー
	BulletManager* bulletManager_;

	// 体力
	int maxHp_ = 500;
	int hp_ = 0;

	// ImGui用状態選択インデックス
	int stateIndex = 0;

	
};

