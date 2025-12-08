#include "EnemyState.h"

#include "Math/Rand.h"
#include "Enemy.h"

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
		enemy->GetBulletManager()->SpawnBullet(laneNumber, bulletSpeed_);
		enemy->ChangeMotionState(std::make_unique<EnemyAttackState>());
		enemy->ChangeTexture(Enemy::Mode::kNormal);
		enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber;

		isAnimation_ = false;
	}
	else
	{
		intervalTime_ += 1.0f / 60.0f;

		if (!isAnimation_ && intervalTime_ >= fireInterval - attackAnimationTime_) {
			isAnimation_ = true;
			enemy->ChangeTexture(Enemy::Mode::kAttack);
			enemy->GetFrontSprite()->SetAnimationTime(attackAnimationTime_);
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
		enemy->GetBulletManager()->SpawnBullet(laneNumber, bulletSpeed_);
		enemy->ChangeMotionState(std::make_unique<EnemyAttackState>());
		enemy->ChangeTexture(Enemy::Mode::kAngry);
		enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber;

		isAnimation_ = false;
	}
	else
	{
		intervalTime_ += 1.0f / 60.0f;

		if (!isAnimation_ && intervalTime_ >= fireInterval - attackAnimationTime_) {
			isAnimation_ = true;
			enemy->ChangeTexture(Enemy::Mode::kAttack);
			enemy->GetFrontSprite()->SetAnimationTime(attackAnimationTime_);
		}
	}
}

void EnemyBerserkState::Exit(Enemy* enemy)
{
}

#pragma endregion



