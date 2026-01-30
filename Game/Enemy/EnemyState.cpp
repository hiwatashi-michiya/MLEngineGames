#include "EnemyState.h"

#include "Math/Rand.h"
#include "Enemy.h"

using namespace MLEngine::Resource;

#pragma region 通常攻撃状態

void EnemyNormalState::Enter(Enemy* enemy)
{
	intervalTime_ = 0.0f;
	prevLaneNumber = -1;
	GlobalVariables* global = GlobalVariables::GetInstance();
	bulletSpeed_ = global->GetFloatValue("EnemyState", "NormalBulletSpeed");
	fireInterval = global->GetFloatValue("EnemyState", "NormalFireInterval");
	isAnimation_ = false;
	normalAnimationTime_ = global->GetFloatValue("EnemyState", "NormalAnimation");
	attackAnimationTime_ = global->GetFloatValue("EnemyState", "NormalAttackAnimation");
	enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kNormal);
	enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kNormal);
	enemyAttackSE_.Load("SE/enemy_attack.mp3");
}

void EnemyNormalState::Update(Enemy* enemy)
{
	if (!EnemyAttackTurnController::GetInstance().CanMyEnemyAttack()) {
		return;
	}


	if (intervalTime_ >= fireInterval)
	{
		int laneNumber = MLEngine::Math::RandomInt(0, 2);

		while (laneNumber == prevLaneNumber)
		{
			laneNumber = MLEngine::Math::RandomInt(0, 2);
		}

		EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(laneNumber, -1);

		enemy->GetBulletManager()->SpawnBullet(laneNumber, bulletSpeed_);
		enemy->ChangeMotionState(std::make_unique<EnemyAttackState>());
		enemy->ChangeTexture(Enemy::Mode::kNormal);
		enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber;
		enemyAttackSE_.Play(Audio::SEVolume);

		isAnimation_ = false;
	}
	else
	{
		intervalTime_ += 1.0f / 60.0f;

		if (!isAnimation_ && intervalTime_ >= fireInterval - attackAnimationTime_) {
			isAnimation_ = true;
			enemy->ChangeTexture(Enemy::Mode::kAttack);
			enemy->GetFrontSprite()->SetAnimationTime(attackAnimationTime_);
			enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kAttack);
			enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kAttack);
		}


	}
}

void EnemyNormalState::Exit(Enemy* enemy)
{
	enemy->ChangeTexture(Enemy::Mode::kNormal);
}

#pragma endregion


#pragma region ダウン状態

void EnemyDownState::Enter(Enemy* enemy)
{
	elapsedTime_ = 0.0f;
	GlobalVariables* global = GlobalVariables::GetInstance();
	downTime = global->GetFloatValue("EnemyState", "DownTime");
}

void EnemyDownState::Update(Enemy* enemy)
{
	EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(-1, -1);

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
	GlobalVariables* global = GlobalVariables::GetInstance();
	bulletSpeed_ = global->GetFloatValue("EnemyState", "BerserkBulletSpeed");
	fireInterval = global->GetFloatValue("EnemyState", "BerserkFireInterval");
	normalAnimationTime_ = global->GetFloatValue("EnemyState", "AngryAnimation");
	attackAnimationTime_ = global->GetFloatValue("EnemyState", "AngryAttackAnimation");
	enemy->ChangeTexture(Enemy::Mode::kAngry);
	enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kAngry);
	enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kAngry);
}

void EnemyBerserkState::Update(Enemy* enemy)
{
	if (!EnemyAttackTurnController::GetInstance().CanMyEnemyAttack()) {
		return;
	}


	if (intervalTime_ >= fireInterval)
	{
		int laneNumber[2];

		laneNumber[0] = MLEngine::Math::RandomInt(0, 2);
		while (laneNumber[0] == prevLaneNumber)
		{
			laneNumber[0] = MLEngine::Math::RandomInt(0, 2);
		}

		laneNumber[1] = MLEngine::Math::RandomInt(0, 2);
		while(laneNumber[1] == laneNumber[0])
		{
			laneNumber[1] = MLEngine::Math::RandomInt(0, 2);
		}

		EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(laneNumber[0], laneNumber[1]);

		enemy->GetBulletManager()->SpawnBullet(laneNumber[0], bulletSpeed_);
		enemy->GetBulletManager()->SpawnBullet(laneNumber[1], bulletSpeed_);
		enemy->ChangeMotionState(std::make_unique<EnemyAttackState>());
		enemy->ChangeTexture(Enemy::Mode::kAngry);
		enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber[0];

		isAnimation_ = false;
	}
	else
	{
		intervalTime_ += 1.0f / 60.0f;

		if (!isAnimation_ && intervalTime_ >= fireInterval - attackAnimationTime_) {
			isAnimation_ = true;
			enemy->ChangeTexture(Enemy::Mode::kAttack);
			enemy->GetFrontSprite()->SetAnimationTime(attackAnimationTime_);
			enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kAttack);
			enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kAttack);
		}
	}
}

void EnemyBerserkState::Exit(Enemy* enemy)
{
}

#pragma endregion



