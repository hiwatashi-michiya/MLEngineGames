#include "EnemyAttackTurnController.h"


void EnemyAttackTurnController::Initialize()
{
#ifdef CLIENT_BUILD
	isServer_ = false;
#else
	isServer_ = true;
#endif

	// サーバー側の敵から開始
	isMyTurn_ = isServer_;
	needSend_ = false;
}

void EnemyAttackTurnController::Update()
{
	ReceiveFromNetwork();
	SendIfNeeded();
}

void EnemyAttackTurnController::OnMyEnemyAttackFinished(int lane)
{
	if (!isMyTurn_) return;

	isMyTurn_ = false;
	needSend_ = true;
	laneNumber_ = lane;
}

void EnemyAttackTurnController::ReceiveFromNetwork()
{
	NetworkManager::EnemyAttackTurnPacket turn{};
	if (!NetworkManager::GetInstance().GetLatestEnemyAttackTurn(turn)) {
		return;
	}

	// 相手が攻撃を終えた → 自分のターン
	if (isServer_ && turn.enemyId == 1) {
		isMyTurn_ = true;
	}
	else if (!isServer_ && turn.enemyId == 0) {
		isMyTurn_ = true;
	}

	// 撃ったレーンを送る
	bulletCaveat_->Warn(turn.lane);
}

void EnemyAttackTurnController::SendIfNeeded()
{
	if (!needSend_) return;

	NetworkManager::EnemyAttackTurnPacket packet{};
	packet.enemyId = isServer_ ? 0 : 1; // 自分が終えた
	packet.lane = laneNumber_;

	NetworkManager::PacketHeader header{};
	header.type = 3;
	header.size = sizeof(NetworkManager::EnemyAttackTurnPacket);

	NetworkManager::GetInstance().Send(header);
	NetworkManager::GetInstance().Send(packet);

	needSend_ = false;
}

