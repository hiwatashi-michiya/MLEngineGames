#pragma once

#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include ".vs/../Engine/Tool/GlobalVariables.h"	


class Enemy;

class EnemyMotionState {
public:

	virtual ~EnemyMotionState() = default;
	virtual void Enter(Enemy* enemy) = 0;
	virtual void Update(Enemy* enemy) = 0;
	virtual void Exit(Enemy* enemy) = 0;

	const float deltaTime_ = 1.0f / 60.0f;
	// 経過時間(秒)
	float elapsedTime_ = 0.0f;
	// 目標時間(秒)
	float targetTime_ = 0.0f;
};

class EnemyIdleState : public EnemyMotionState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;
private:
	

};

class EnemyOnHitState : public EnemyMotionState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	// 回転角度
	float degrees_ = 0.0f;
	// 最大・最小角度
	float limitDegrees_ = 45.0f;
	// 角度加算値
	float addDegrees_ = 5.0f;
};

class EnemyAttackState : public EnemyMotionState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	MLEngine::Math::Vector3 originalPosition_;
	MLEngine::Math::Vector2 shakeOffset_{0.01f, 0.01f};
};

class EnemyknockDownState : public EnemyMotionState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	// 回転
	MLEngine::Math::Vector3 rotate_;
	// セクション割合
	float rotateSection_ = 0.2f;
	float liedownSection_ = 0.3f;
	float getupSection_ = 0.8f;
	// 横たわる角度
	float lieDownAngle_ = 70.0f;

};

