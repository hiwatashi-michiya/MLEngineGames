#include "EnemyState.h"

#include "Math/Rand.h"
#include "Enemy.h"

#pragma region 通常攻撃状態

void EnemyNormalState::Enter(Enemy* enemy)
{
	intervalTime_ = 0.0f;
	prevLaneNumber = -1;
	GlobalVariables* global_ = GlobalVariables::GetInstance();
	bulletSpeed_ = global_->GetFloatValue("EnemyState", "NormalBulletSpeed");
	fireInterval = global_->GetFloatValue("EnemyState", "NormalFireInterval");
}

void EnemyNormalState::Update(Enemy* enemy)
{
	if (intervalTime_ >= fireInterval)
	{
		int laneNumber = MLEngine::Math::RandomInt(0, 2);

		while (laneNumber == prevLaneNumber)
		{
			laneNumber = MLEngine::Math::RandomInt(0, 2);
		}
		//MLEngine::Math::Vector2 launchPos = enemy->LaunchPosition();
		//enemy->GetBulletManger()->SpawnBullet({ launchPos.x + enemy->GetDistance() * (laneNumber - 1), launchPos.y}, {(laneNumber - 1.0f) * wide_ , 1.0f}, speed_);
		//enemy->GetBulletManger()->SpawnBullet(launchPos, laneNumber, bulletSpeed_);
		enemy->GetBulletManger()->SpawnBullet(laneNumber, bulletSpeed_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber;
	}
	else
	{
		intervalTime_ += 1.0f / 60.0f;
	}
}

void EnemyNormalState::Exit(Enemy* enemy)
{
}

#pragma endregion


#pragma region ダウン状態

void EnemyDownState::Enter(Enemy* enemy)
{
	elapsedTime_ = 0.0f;
	GlobalVariables* global_ = GlobalVariables::GetInstance();
	downTime = global_->GetFloatValue("EnemyState", "DownTime");
}

void EnemyDownState::Update(Enemy* enemy)
{
	elapsedTime_ += 1.0f / 60.0f;
	if (elapsedTime_ >= downTime)
	{
		enemy->ChangeState(std::make_unique<EnemyNormalState>());
	}
}

void EnemyDownState::Exit(Enemy* enemy)
{
}

#pragma endregion


#pragma region 猛攻状態

void EnemyBerserkState::Enter(Enemy* enemy)
{
	intervalTime_ = 0.0f;
	prevLaneNumber = -1;
	GlobalVariables* global_ = GlobalVariables::GetInstance();
	bulletSpeed_ = global_->GetFloatValue("EnemyState", "BerserkBulletSpeed");
	fireInterval = global_->GetFloatValue("EnemyState", "BerserkFireInterval");
}

void EnemyBerserkState::Update(Enemy* enemy)
{
	if (intervalTime_ >= fireInterval)
	{
		int laneNumber = MLEngine::Math::RandomInt(0, 2);

		while (laneNumber == prevLaneNumber)
		{
			laneNumber = MLEngine::Math::RandomInt(0, 2);
		}
		//MLEngine::Math::Vector2 launchPos = enemy->LaunchPosition();
		//enemy->GetBulletManger()->SpawnBullet({ launchPos.x + enemy->GetDistance() * (laneNumber - 1), launchPos.y }, { (laneNumber - 1.0f) * wide_ , 1.0f }, speed_);
		//enemy->GetBulletManger()->SpawnBullet({ launchPos.x + enemy->GetDistance() * (laneNumber - 1), launchPos.y }, laneNumber, bulletSpeed_);
		enemy->GetBulletManger()->SpawnBullet(laneNumber, bulletSpeed_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber;
	}
	else
	{
		intervalTime_ += 1.0f / 60.0f;
	}
}

void EnemyBerserkState::Exit(Enemy* enemy)
{
}

#pragma endregion



