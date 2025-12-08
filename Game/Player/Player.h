#pragma once
#include"Character/BaseCharacter.h"
#include"VirtualController.h"
#include<../Network/NetworkManager.h>
#include<Engine/Tool/GlobalVariables.h>

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

	bool GetIsJustMoved() const {
		return isJustMoved_;
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

	std::unique_ptr<MLEngine::Resource::Sprite> sprite_;

	MLEngine::Resource::Texture frontTexture_;
	MLEngine::Resource::Texture backTexture_;

		
	bool isTitleScene_ = false;
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

};


