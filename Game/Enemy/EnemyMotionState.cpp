#include "EnemyMotionState.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <corecrt_math_defines.h>
#include <format>

#include "Enemy/Enemy.h"
#include "Math/Rand.h"
#include"Externals/imgui/imgui.h"

float DegToRad(float degrees) {
	return degrees * (float)(M_PI / 180.0f);
}

#pragma region 通常

void EnemyIdleState::Enter(Enemy* enemy)
{

}

void EnemyIdleState::Update(Enemy* enemy)
{
}

void EnemyIdleState::Exit(Enemy* enemy)
{
}

#pragma endregion

#pragma region ダメージ
void EnemyOnHitState::Enter(Enemy* enemy)
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("EnemyMotionState");

	global->AddItem("EnemyMotionState", "OnHit", 2.0f);
	global->AddItem("EnemyMotionState", "AddDegrees", 5.0f);
	global->AddItem("EnemyMotionState", "LimitDegrees", 45.0f);

	elapsedTime_ = 0.0f;
	targetTime_ = global->GetFloatValue("EnemyMotionState", "OnHit");
	degrees_ = 0.0f;
	addDegrees_ = global->GetFloatValue("EnemyMotionState", "AddDegrees");
	targetTime_ = 2.0f;
	limitDegrees_ = global->GetFloatValue("EnemyMotionState", "LimitDegrees");
}

void EnemyOnHitState::Update(Enemy* enemy)
{
	if(elapsedTime_ < targetTime_) {
		elapsedTime_ += deltaTime_;
	}
	else {
		enemy->ChangeMotionState(std::make_unique<EnemyIdleState>());
		return;
	}

	degrees_ += addDegrees_;

	if(degrees_ > limitDegrees_ || degrees_ < -limitDegrees_) {
		addDegrees_ *= -1.0f;
	}

	enemy->SetRotate({ DegToRad(degrees_), 0.0f, 0.0f });

}


void EnemyOnHitState::Exit(Enemy* enemy)
{
	enemy->SetRotate({ 0.0f, 0.0f, 0.0f });
}
#pragma endregion


#pragma region 攻撃
void EnemyAttackState::Enter(Enemy* enemy)
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("EnemyMotionState");

	global->AddItem("EnemyMotionState", "Attack", 0.5f);
	global->AddItem("EnemyMotionState", "ShakeOffset", MLEngine::Math::Vector2{ 0.01f, 0.01f });


	elapsedTime_ = 0.0f;
	targetTime_ = global->GetFloatValue("EnemyMotionState", "Attack");
	shakeOffset_ = global->GetVector2Value("EnemyMotionState", "ShakeOffset");
	originalPosition_ = enemy->GetCamera()->position_;
}

void EnemyAttackState::Update(Enemy* enemy)
{
	if (elapsedTime_ < targetTime_) {
		elapsedTime_ += deltaTime_;
	}
	else {
		enemy->ChangeMotionState(std::make_unique<EnemyIdleState>());
		return;
	}

	enemy->GetCamera()->position_.x = originalPosition_.x + MLEngine::Math::RandomFloat(-shakeOffset_.x, shakeOffset_.x);
	enemy->GetCamera()->position_.y = originalPosition_.y + MLEngine::Math::RandomFloat(-shakeOffset_.y, shakeOffset_.y);


}

void EnemyAttackState::Exit(Enemy* enemy)
{
	enemy->GetCamera()->position_ = originalPosition_;

}
#pragma endregion

#pragma region ダウン
void EnemyknockDownState::Enter(Enemy* enemy)
{
	GlobalVariables* global = GlobalVariables::GetInstance();
	global->CreateGroup("EnemyMotionState");

	global->AddItem("EnemyMotionState", "RotateSection", 0.2f);
	global->AddItem("EnemyMotionState", "LieDownSection", 0.3f);
	global->AddItem("EnemyMotionState", "GetUpSection", 0.8f);
	global->AddItem("EnemyMotionState", "LieDownAngle_", 70.0f);

	elapsedTime_ = 0.0f;
	targetTime_ = global->GetFloatValue("EnemyState", "DownTime");
	rotate_ = { 0.0f, 0.0f, 0.0f };
	rotateSection_ = global->GetFloatValue("EnemyMotionState", "RotateSection");
	liedownSection_ = global->GetFloatValue("EnemyMotionState", "LieDownSection");
	getupSection_ = global->GetFloatValue("EnemyMotionState", "GetUpSection");
	lieDownAngle_ = global->GetFloatValue("EnemyMotionState", "LieDownAngle_");
}

void EnemyknockDownState::Update(Enemy* enemy)
{
	if (elapsedTime_ < targetTime_) {
		elapsedTime_ += deltaTime_;
	}
	else {
		enemy->ChangeMotionState(std::make_unique<EnemyIdleState>());
		return;
	}

	if (elapsedTime_ < targetTime_ * rotateSection_) {
		
		rotate_.y = DegToRad(360.0f * (elapsedTime_ / (targetTime_ * rotateSection_)));
	}
	if (elapsedTime_ < targetTime_ * liedownSection_) {
		rotate_.x = DegToRad(lieDownAngle_ * (elapsedTime_ / (targetTime_ * liedownSection_)));
	}
	else if (elapsedTime_ > targetTime_ * getupSection_) {
		rotate_.x = DegToRad(lieDownAngle_ + (0.0f - lieDownAngle_) * ((elapsedTime_ - targetTime_ * getupSection_) / (targetTime_ - targetTime_ * getupSection_)));
	}

	enemy->SetRotate(rotate_);

}

void EnemyknockDownState::Exit(Enemy* enemy)
{
	enemy->SetRotate(rotate_);
}
#pragma endregion