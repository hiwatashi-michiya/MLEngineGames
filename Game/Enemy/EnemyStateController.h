#pragma once

#include ".vs/../Engine/Tool/GlobalVariables.h"
#include<../Network/NetworkManager.h>

class Enemy;

class EnemyStateController {
public:
    static EnemyStateController& GetInstance() {
        static EnemyStateController instance;
        return instance;
    }

    // 初期化
    void Initialize();

    // 毎フレーム呼ぶ
    void Update();

    // 攻撃が確定した瞬間に呼ぶ
    void OnMyEnemyStateFinished(bool great, bool angry);

	bool GetGreatAttackFlag() const { return greatAttackFlag_; }
    bool GetAngryFlag() const { return angryAttackFlag_; }


private:
    EnemyStateController() = default;

    // 受信処理
    void ReceiveFromNetwork();

    // 送信処理
    void SendIfNeeded();
private:
    bool isServer_ = false;
    bool needSend_ = false;
	bool greatAttackFlag_;
    bool angryAttackFlag_;
};