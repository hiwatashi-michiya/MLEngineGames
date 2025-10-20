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
	void Update(float deltaTime) override;
	//描画
	void Draw() override;
	//デバック描画(ラインとか)
	void DebugDraw() override;

private:
	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;

	MLEngine::Resource::Sprite* sprite_ = nullptr;

	MLEngine::Resource::Texture texture_;

	//前を向いているか
	bool isforward_ = true;
	//体力最大値
	int16_t lifeMax_ = 100;
	//回復速度
	int16_t recoverySpeed_ = 60;
	//回復量
	int16_t recoveryValue_ = 1;
	//現在いるラインの番号
	int8_t nowLine_ = 2;
	//レーンの幅
	int16_t posXValue_ = 200;

};


