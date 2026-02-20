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
	maxGrateAttackTime_ = global->GetFloatValue("EnemyState", "MaxGreatAttackTime");
	maxAngryTime_ = global->GetFloatValue("EnemyState", "MaxAngryTime");

	enemy->ChangeTexture(Enemy::Mode::kNormal);
	enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);

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

		EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(laneNumber, -1, false);

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


		if(EnemyStateController::GetInstance().GetGreatAttackFlag())
		{
			enemy->AddGrateAttackTime(1.0f / 60.0f);
		}
		if(EnemyStateController::GetInstance().GetAngryFlag())
		{
 			enemy->AddAngryTime(1.0f / 60.0f);
		}

		if (intervalTime_ < fireInterval - attackAnimationTime_) {
			if (enemy->GetGreatAttackTime() > maxGrateAttackTime_) {
				enemy->ChangeState(std::make_unique<EnemyGreatAttackState>());
				enemy->ResetGreatAttackTime();
				enemy->GetBulletManager()->GetBulletCaveat()->Continuous();
			}
			else if (enemy->GetAngryTime() > maxAngryTime_) {
				enemy->ChangeState(std::make_unique<EnemyBerserkState>());
				enemy->ResetAngryTime();
				
			}

		}

		if (!isAnimation_ && intervalTime_ >= fireInterval - attackAnimationTime_) {
			isAnimation_ = true;
			enemyAttackSE_.Play(Audio::SEVolume);
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
	enemy->ChangeMotionState(std::make_unique<EnemyknockDownState>());
	enemy->ChangeTexture(Enemy::Mode::kCry);
}

void EnemyDownState::Update(Enemy* enemy)
{
	EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(-1, -1, false);
	EnemyStateController::GetInstance().OnMyEnemyStateFinished(false, false);

	/*elapsedTime_ += 1.0f / 60.0f;
	if (elapsedTime_ >= downTime)
	{
		enemy->ChangeState(std::make_unique<EnemyNormalState>());
	}*/
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
	totalTime_ = global->GetFloatValue("EnemyState", "AngryTotalTime");
	bulletSpeed_ = global->GetFloatValue("EnemyState", "BerserkBulletSpeed");
	fireInterval = global->GetFloatValue("EnemyState", "BerserkFireInterval");
	normalAnimationTime_ = global->GetFloatValue("EnemyState", "AngryAnimation");
	attackAnimationTime_ = global->GetFloatValue("EnemyState", "AngryAttackAnimation");
	enemy->ChangeTexture(Enemy::Mode::kAngry);
	enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
	enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kAngry);
	enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kAngry);
	enemyAttackSE_.Load("SE/enemy_attack.mp3");
}

void EnemyBerserkState::Update(Enemy* enemy)
{
	if (!EnemyAttackTurnController::GetInstance().CanMyEnemyAttack()) {
		if (enemy->GetLeftHand()->GetHandState() != EnemyHand::HandState::kAttack && time_ > totalTime_)
		{
			enemy->ChangeState(std::make_unique<EnemyNormalState>());


			EnemyStateController::GetInstance().OnMyEnemyStateFinished(EnemyStateController::GetInstance().GetGreatAttackFlag(), false);
			enemy->ResetAngryTime();
		}

		return;
	}

	time_ += 1.0f / 60.0f;
	intervalTime_ += 1.0f / 60.0f;

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

		EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(laneNumber[0], laneNumber[1], true);
		enemy->GetBulletManager()->SpawnBullet(laneNumber[0], bulletSpeed_);
		enemy->GetBulletManager()->SpawnBullet(laneNumber[1], bulletSpeed_);
		enemy->ChangeMotionState(std::make_unique<EnemyAttackState>());
		enemy->ChangeTexture(Enemy::Mode::kAngry);
		enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
		intervalTime_ = 0.0f;
		prevLaneNumber = laneNumber[0];

		isAnimation_ = false;
		attackCount_++;
	}
	else
	{

		if (!isAnimation_ && intervalTime_ >= fireInterval - attackAnimationTime_) {
			isAnimation_ = true;
			enemyAttackSE_.Play(Audio::SEVolume);
			enemy->ChangeTexture(Enemy::Mode::kAttack);
			enemy->GetFrontSprite()->SetAnimationTime(attackAnimationTime_);
			enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kAttack);
			enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kAttack);
		}
	}

	if (attackCount_ == totalTime_ / fireInterval)
	{
		EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(-1, -1, false);
	}
}

void EnemyBerserkState::Exit(Enemy* enemy)
{
	enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kNormal);
	enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kNormal);
	EnemyStateController::GetInstance().OnMyEnemyStateFinished(EnemyStateController::GetInstance().GetGreatAttackFlag(), false);

}

#pragma endregion

void EnemyGreatAttackState::Enter(Enemy* enemy)
{
	intervalTime_ = 0.0f;
	currentAttackCount_ = 0;
	attackCount_ = GlobalVariables::GetInstance()->GetIntValue("EnemyState", "GreatAttackCount");
	normalAnimationTime_ = GlobalVariables::GetInstance()->GetFloatValue("EnemyState", "NormalAnimation");
	attackAnimationTime_ = GlobalVariables::GetInstance()->GetFloatValue("EnemyState", "AngryAttackAnimation");
	continuousTime_ = GlobalVariables::GetInstance()->GetFloatValue("BulletCaveat", "ContinuousTotalTime");
	laneNumber_[0] = MLEngine::Math::RandomInt(0, 2);
	while(laneNumber_[0] == laneNumber_[1])
	{
		laneNumber_[1] = MLEngine::Math::RandomInt(0, 2);
	}
	enemyAttackSE_.Load("SE/enemy_attack.mp3");
}

void EnemyGreatAttackState::Update(Enemy* enemy)
{
	if (!EnemyAttackTurnController::GetInstance().CanMyEnemyAttack()) {
		return;
	}


	if (waitTime_ < continuousTime_) {
		waitTime_ += 1.0f / 60.0f;
		return;
	}

	if (currentAttackCount_ > attackCount_) {
		EnemyStateController::GetInstance().OnMyEnemyStateFinished(false, EnemyStateController::GetInstance().GetAngryFlag());
		enemy->ChangeState(std::make_unique<EnemyNormalState>());
	}
	else if (intervalTime_ >= fireInterval)
	{
		enemy->GetBulletManager()->SpawnBullet(laneNumber_[0], bulletSpeed_);
		enemy->GetBulletManager()->SpawnBullet(laneNumber_[1], bulletSpeed_);
		enemy->ChangeMotionState(std::make_unique<EnemyAttackState>());
		EnemyAttackTurnController::GetInstance().OnMyEnemyAttackFinished(laneNumber_[0], laneNumber_[1], true);
		enemy->ChangeTexture(Enemy::Mode::kNormal);
		enemy->GetFrontSprite()->SetAnimationTime(normalAnimationTime_);
		intervalTime_ = 0.0f;

		isAnimation_ = false;

		currentAttackCount_++;
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

void EnemyGreatAttackState::Exit(Enemy* enemy)
{
	enemy->GetLeftHand()->SetHandState(EnemyHand::HandState::kNormal);
	enemy->GetRightHand()->SetHandState(EnemyHand::HandState::kNormal);
	EnemyStateController::GetInstance().OnMyEnemyStateFinished(false, EnemyStateController::GetInstance().GetAngryFlag());
}
