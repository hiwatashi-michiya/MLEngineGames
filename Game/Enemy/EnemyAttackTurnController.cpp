#include "EnemyAttackTurnController.h"
#include "EnemyStateController.h"


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

void EnemyAttackTurnController::OnMyEnemyAttackFinished(int lane0, int lane1, bool isAngry)
{
	if (!isMyTurn_) return;

	isMyTurn_ = false;
	needSend_ = true;
	laneNumber_[0] = lane0;
	laneNumber_[1] = lane1;
	isAngry_ = isAngry;
}

void EnemyAttackTurnController::ReceiveFromNetwork()
{
	NetworkManager::EnemyAttackTurnPacket turn{};
	if (!NetworkManager::GetInstance().GetLatestEnemyAttackTurn(turn)) {
		return;
	}

	isMyTurn_ = turn.isShot;

	if (turn.isAngry) {
		OnMyEnemyAttackFinished(-1, -1, false);
	}

	
}

void EnemyAttackTurnController::SendIfNeeded()
{
	if (!needSend_) return;

	NetworkManager::EnemyAttackTurnPacket packet{};
	//packet.enemyId = isServer_ ? 0 : 1; // 自分が終えた
	packet.isShot = !isMyTurn_;
	packet.lane0 = laneNumber_[0];
	packet.lane1 = laneNumber_[1];
	packet.isAngry = isAngry_;

	NetworkManager::PacketHeader header{};
	header.type = 3;
	header.size = sizeof(NetworkManager::EnemyAttackTurnPacket);

	NetworkManager::GetInstance().Send(header);
	NetworkManager::GetInstance().Send(packet);

	needSend_ = false;
}

