#pragma once

#include ".vs/../Engine/Tool/GlobalVariables.h"	
#include "Enemy/EnemyAttackTurnController.h"
#include "Audio/Audio.h"
#include "Enemy/EnemyStateController.h"

class Enemy;

class EnemyState {
public:
	virtual ~EnemyState() = default;
	virtual void Enter(Enemy* enemy) = 0;
	virtual void Update(Enemy* enemy) = 0;
	virtual void Exit(Enemy* enemy) = 0;
};

class EnemyNormalState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	//発射間隔(秒)
	float fireInterval = 3.0f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	// 前回のレーン番号
	int prevLaneNumber = -1;
	// 移動量
	const float speed_ = 5.0f;
	const float wide_ = 0.5f;
	// 弾の移動速度	
	float bulletSpeed_ = 2.0f;
	// アニメーションする時間
	float normalAnimationTime_;
	float attackAnimationTime_;
	bool isAnimation_;
	//SE
	MLEngine::Resource::Audio enemyAttackSE_;
	// 
	float maxGrateAttackTime_ = 20.0f;
	float maxAngryTime_ = 6.0f;
	float continousTime_ = 3.0f;
};

class EnemyDownState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	//ダウン時間(秒)
	float downTime = 15.0f;
	float elapsedTime_ = 0.0f;
};

class EnemyBerserkState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	//発射間隔(秒)
	float fireInterval = 1.5f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	float totalTime_ = 3.0f;
	float time_ = 0.0f;
	int attackCount_ = 0;
	// 前回のレーン番号
	int prevLaneNumber = -1;
	// 移動量
	const float speed_ = 6.0f;
	const float wide_ = 0.75f;
	// 弾の移動速度
	float bulletSpeed_ = 1.5f;
	// アニメーション時間
	float normalAnimationTime_;
	float attackAnimationTime_;
	bool isAnimation_;
	//SE
	MLEngine::Resource::Audio enemyAttackSE_;
};

class EnemyGreatAttackState : public EnemyState {
	public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	
	//発射間隔(秒)
	float fireInterval = 0.3f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	float waitTime_ = 0.0f;
	float continuousTime_ = 3.0f;
	// 移動量
	const float speed_ = 7.0f;
	const float wide_ = 1.0f;
	// 弾の移動速度
	float bulletSpeed_ = 1.0f;

	// 攻撃回数
	int attackCount_ = 7;
	int currentAttackCount_ = 0;

	int laneNumber_[2];

	// アニメーション時間
	float normalAnimationTime_;
	float attackAnimationTime_;
	bool isAnimation_;
	//SE
	MLEngine::Resource::Audio enemyAttackSE_;
};

class EnemyTutorialNormalState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	//発射間隔(秒)
	float fireInterval = 3.0f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	// 前回のレーン番号
	int prevLaneNumber = -1;
	// 移動量
	const float speed_ = 5.0f;
	const float wide_ = 0.5f;
	// 弾の移動速度	
	float bulletSpeed_ = 2.0f;
	// アニメーションする時間
	float normalAnimationTime_;
	float attackAnimationTime_;
	bool isAnimation_;
	//SE
	MLEngine::Resource::Audio enemyAttackSE_;
	// 
	float maxGrateAttackTime_ = 20.0f;
	float maxAngryTime_ = 6.0f;
	float continousTime_ = 3.0f;
};

class EnemyTutorialBerserkState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;

	//発射間隔(秒)
	float fireInterval = 1.5f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	float totalTime_ = 3.0f;
	float time_ = 0.0f;
	int attackCount_ = 0;
	// 前回のレーン番号
	int prevLaneNumber = -1;
	// 移動量
	const float speed_ = 6.0f;
	const float wide_ = 0.75f;
	// 弾の移動速度
	float bulletSpeed_ = 1.5f;
	// アニメーション時間
	float normalAnimationTime_;
	float attackAnimationTime_;
	bool isAnimation_;
	//SE
	MLEngine::Resource::Audio enemyAttackSE_;
};