#pragma once
#include"Character/BaseCharacter.h"
//プレイヤーが操作する自機
class Player : public BaseCharacter{
public:
	Player();
	~Player();

	//初期化
	void Initialize() override;
	//終了処理
	void Finalize() override;
	//更新
	void Update(const float deltaTime) override;
	//描画
	void Draw() override;
	//デバック描画(ラインとか)
	void DebugDraw() override;

private:
	//プレイヤーのボタンによる操作
	void PlayerMove();
	//時間関連の処理
	void TimeProcess(const float deltaTime);

	//今いるレーンに応じての座標計算
	float LaneSpecificCalculation();
	//時間による回復
	void PlayerRecovery();

private:
	GameConfig* config_ = nullptr;

	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;

	MLEngine::Resource::Sprite* sprite_ = nullptr;

	MLEngine::Resource::Texture texture_;

	//前を向いているか
	bool isforward_ = true;
	//前を向いているか
	bool isLifeMax_ = true;
	//攻撃をくらったか
	bool isDamaged_ = false;

	float damageTime_ = 0.0f;

	float damegeCount_ = 3.0f;

	float time_ = 0.0f;

	//体力最大値
	int lifeMax_ = 100;
	//回復速度
	float recoverySpeed_ = 1.0f;
	//回復量
	int recoveryValue_ = 1;
	//現在いるラインの番号
	int nowLine_ = 2;

};


