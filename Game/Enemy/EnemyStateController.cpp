#include "EnemyStateController.h"

void EnemyStateController::Initialize()
{
#ifdef CLIENT_BUILD
	isServer_ = false;
#else
	isServer_ = true;
#endif

	// サーバー側の敵から開始
	/*if (isServer_) {
		greatAttackFlag_ = 0;
		angryAttackFlag_ = 0;
	}
	else {
		greatAttackFlag_ = 1;
		angryAttackFlag_ = 1;
	}*/
	greatAttackFlag_ = isServer_;
	angryAttackFlag_ = isServer_;
	needSend_ = false;
}

void EnemyStateController::Update()
{
   	ReceiveFromNetwork();
	SendIfNeeded();
}

void EnemyStateController::OnMyEnemyStateFinished(bool great, bool angry)
{
	needSend_ = true;
	greatAttackFlag_ = great;
	angryAttackFlag_ = angry;
}

void EnemyStateController::ReceiveFromNetwork()
{
	NetworkManager::EnemyStatePacket turn{};
	if (!NetworkManager::GetInstance().GetLatestEnemyState(turn)) {
		return;
	}
	// 攻撃が確定した瞬間の情報を受け取る

	if (turn.greatAttackFlag) {
		greatAttackFlag_ = true;
	}

	if (turn.angryAttackFlag) {
		angryAttackFlag_ = true;
	}
}

void EnemyStateController::SendIfNeeded()
{
	if (!needSend_) return;
	NetworkManager::PacketHeader header{};
	header.type = 5;
	header.size = sizeof(NetworkManager::EnemyStatePacket);
	NetworkManager::EnemyStatePacket statePacket{};
	/*statePacket.header.type = 4;
	statePacket.header.size = sizeof(NetworkManager::EnemyStatePacket);*/
	statePacket.greatAttackFlag = !greatAttackFlag_;
	statePacket.angryAttackFlag = !angryAttackFlag_;
	NetworkManager::GetInstance().Send(header);
	NetworkManager::GetInstance().Send(statePacket);
	needSend_ = false;
}