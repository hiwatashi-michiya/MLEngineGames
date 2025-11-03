#pragma once

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

private:
	//発射間隔(秒)
	const float fireInterval = 3.0f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	// 前回のレーン番号
	int prevLaneNumber = -1;
	// 移動量
	const float speed_ = 5.0f;
	const float wide_ = 0.5f;
};

class EnemyDownState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;
private:
	//ダウン時間(秒)
	const float downTime = 15.0f;
	float elapsedTime_ = 0.0f;
};

class EnemyBerserkState : public EnemyState {
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;
private:
	//発射間隔(秒)
	const float fireInterval = 1.5f;
	//経過時間(秒)
	float intervalTime_ = 0.0f;
	// 前回のレーン番号
	int prevLaneNumber = -1;
	// 移動量
	const float speed_ = 6.0f;
	const float wide_ = 0.75f;
};;