#pragma once
#include"Character/BaseCharacter.h"
#include"VirtualController.h"
#include<../Network/NetworkManager.h>
#include<Engine/Tool/GlobalVariables.h>
#include <Sprite3D.h>
#include "Audio/Audio.h"

#include "../Joycon/Joycon.h"

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

	void OnCollision(const int damege);

	int GetNowLine()const {
		return nowLine_;
	}

	bool GetIsForward() const {
		return isForward_;
	}

	bool GetIsDead() const {
		return isDead_;
	}


	NetworkManager::SendPlayerState GetSendPlayerState() const {
		return plState_;
	}

	int GetLifeMax()const {
		return lifeMax_;
	}

	int GetLife()const {
		return life_;
	}

	float GetLifeRatio() const {
		return lifeRatio_;
	}

	bool GetIsJustMoved() const {
		return isJustMoved_;
	}

	bool GetIsDamaged() const {
		return isDamaged_;
	}

	bool GetIsJustTurned() const {
		return isJustTurned_;
	}


	void SetSendPlayerState(const NetworkManager::SendPlayerState plState){
		plState_ = plState;
	}

	void SetIsTitleScene(bool isTitleScene) {
		isTitleScene_ = isTitleScene;
	}

	void SetIsResultScene(bool flag) {
		isResultScene_ = flag;
	}

	//回復雲のレーンにいるか
	void ChackInRecoveryArea(int RecoveryArea) {
		if (nowLine_ == RecoveryArea){
			isRecoveryArea_ = true;
		}
		else {
			isRecoveryArea_ = false;
		}

		
	}

private:
	//プレイヤーのボタンによる操作
	void PlayerMove();
	//時間関連の処理
	void TimeProcess(const float deltaTime);

	//今いるレーンに応じての座標計算
	float LaneSpecificCalculation();
	//時間による回復
	void PlayerRecovery();
	//送る情報を更新
	void PlayerInfoInsertion();

	//通信相手のplayer情報を取得
	void SyncFromNetwork();

	//瞬間を記録する変数を初期化
	void ResetEvents();

private:
	GameConfig* config_ = nullptr;

	NetworkManager::SendPlayerState plState_{};

	//入力デバイス
	VirtualController* vController_ = nullptr;

	MLEngine::Input::Manager* input_ = nullptr;

	MLEngine::Resource::Sprite3D sprite3D_;

	std::string frontTextureName_;
	std::string backTextureName_;
	std::string damageTextureName_;

	MLEngine::Math::Vector3 resultPosition_{};

		
	bool isTitleScene_ = false;
	bool isResultScene_ = false;
	//前を向いているか
	bool isForward_ = true;
	//体力が最大かどうか
	bool isLifeMax_ = true;
	
	bool isDead_ = false;
	//攻撃をくらったか
	bool isDamaged_ = false;
	//瞬間を記録する
	bool isJustTurned_ = false;
	bool isJustMoved_ = false;

	bool isRecoveryArea_ = false;

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


	int bulletDamege_ = 10;
	//体力の残り割合
	float lifeRatio_ = 1.0f;
	//ダメージ時の点滅感覚
	float damageBlinkingTime_ = 0.1f;
	//点滅時間カウント
	float damageBlinkingCount_ = 0.0f;
	//ジャングル
	std::unique_ptr<Joycon> joyconInput;

	//SE
	MLEngine::Resource::Audio playerMoveSE_;
	MLEngine::Resource::Audio playerTurnSE_;
	MLEngine::Resource::Audio playerBounceSE_;
	MLEngine::Resource::Audio playerDamageSE_;


};


