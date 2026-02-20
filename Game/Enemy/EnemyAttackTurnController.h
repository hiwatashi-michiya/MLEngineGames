#pragma once
#include <atomic>
#include<../Network/NetworkManager.h>
#include <../Game/Bullet/BulletCaveat.h>

class EnemyAttackTurnController {
public:
    static EnemyAttackTurnController& GetInstance() {
        static EnemyAttackTurnController instance;
        return instance;
    }

    // 初期化
    void Initialize();

    // 毎フレーム呼ぶ
    void Update();

    // Enemy から呼ばれる
    bool CanMyEnemyAttack() const {
        return isMyTurn_;
    }

    // 攻撃が確定した瞬間に呼ぶ
    void OnMyEnemyAttackFinished(int lane0, int lane1, bool isAngry);

	void SetEnemyHP(int hp) { enemyHP_ = hp; }

   // void SetBulletCaveat(BulletCaveat* bulletCaveat) { bulletCaveat_ = bulletCaveat; }

private:
    EnemyAttackTurnController() = default;

    // 受信処理
    void ReceiveFromNetwork();

    // 送信処理
    void SendIfNeeded();
private:
    bool isServer_ = false;
    std::atomic<bool> isMyTurn_{ false };
    bool needSend_ = false;
	int laneNumber_[2];
	bool isAngry_ = false;
    int enemyHP_ = 0;
    //BulletCaveat* bulletCaveat_;

};