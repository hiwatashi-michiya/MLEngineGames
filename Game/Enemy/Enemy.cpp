#include "Enemy.h"

#include <format>

#include "Input/Input.h"
#include"Externals/imgui/imgui.h"

void Enemy::Initialize()
{
	global_ = GlobalVariables::GetInstance();

	scale_ = global_->GetVector3Value("EnemyState", "Scale");
	translate_ = global_->GetVector3Value("EnemyState", "Translate");
	maxHp_ = global_->GetIntValue("EnemyState", "MaxHp");
	hp_ = global_->GetIntValue("EnemyState", "MaxHp");

	global_->AddItem("EnemyState", "MaxDownCount", maxDownCount_);
	maxDownCount_ = global_->GetIntValue("EnemyState", "MaxDownCount");

	model_.Initialize("./Resources/model/plane/plane.obj");
	model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, translate_);

	ChangeState(std::make_unique<EnemyNormalState>());

	hp_ = maxHp_;

	downCount_ = 0;

	enemyUI_ = std::make_unique<EnemyUI>();
	enemyUI_->Initialize(this);

}

void Enemy::Update()
{
	if (!dynamic_cast<EnemyBerserkState*>(currentState_.get())) {
		if(hp_ <= maxHp_ * 0.3f) {
			ChangeState(std::make_unique<EnemyBerserkState>());
		}
	}

	if (!dynamic_cast<EnemyDownState*>(currentState_.get())) {
		if(downCount_ >= maxDownCount_) {
			ChangeState(std::make_unique<EnemyDownState>());
			downCount_ = 0;
		}
	}

	MLEngine::Input::Manager* input = MLEngine::Input::Manager::GetInstance();

	currentState_->Update(this);

	enemyUI_->Update();

#ifdef _DEBUG

	ImGui::Begin("敵パラメーター");

	// 状態
	if (ImGui::Combo("モード", &stateIndex, states, IM_ARRAYSIZE(states))) {
		if (stateIndex == 0) {
			ChangeState(std::make_unique<EnemyNormalState>());
		}
		else if (stateIndex == 1) {
			ChangeState(std::make_unique<EnemyDownState>());
		}
		else if (stateIndex == 2) {
			ChangeState(std::make_unique<EnemyBerserkState>());
		}
	}
	// 状態ごとのパラメーター
	if (dynamic_cast<EnemyNormalState*>(currentState_.get())) { // 通常状態
		ImGui::DragFloat("弾速度", &dynamic_cast<EnemyNormalState*>(currentState_.get())->bulletSpeed_, 0.1f);
		ImGui::DragFloat("発射間隔", &dynamic_cast<EnemyNormalState*>(currentState_.get())->fireInterval, 0.1f);
		ImGui::Text("経過時間 : %f" ,dynamic_cast<EnemyNormalState*>(currentState_.get())->intervalTime_);
		global_->datas_["EnemyState"].items["NormalBulletSpeed"].value = dynamic_cast<EnemyNormalState*>(currentState_.get())->bulletSpeed_;
		global_->datas_["EnemyState"].items["NormalFireInterval"].value = dynamic_cast<EnemyNormalState*>(currentState_.get())->fireInterval;
		stateIndex = 0;
	}
	else if (dynamic_cast<EnemyDownState*>(currentState_.get())) { // ダウン状態
		ImGui::DragFloat("ダウン時間", &dynamic_cast<EnemyDownState*>(currentState_.get())->downTime, 0.1f);
		ImGui::Text("経過時間 : %f", dynamic_cast<EnemyDownState*>(currentState_.get())->elapsedTime_);
		global_->datas_["EnemyState"].items["DownTime"].value = dynamic_cast<EnemyDownState*>(currentState_.get())->downTime;
		stateIndex = 1;
	}
	else if (dynamic_cast<EnemyBerserkState*>(currentState_.get())) { // 猛攻状態
		ImGui::DragFloat("弾速度", &dynamic_cast<EnemyBerserkState*>(currentState_.get())->bulletSpeed_, 0.1f);
		ImGui::DragFloat("発射間隔", &dynamic_cast<EnemyBerserkState*>(currentState_.get())->fireInterval, 0.1f);
		ImGui::Text("経過時間 : %f", dynamic_cast<EnemyBerserkState*>(currentState_.get())->intervalTime_);
		global_->datas_["EnemyState"].items["BerserkBulletSpeed"].value = dynamic_cast<EnemyBerserkState*>(currentState_.get())->bulletSpeed_;
		global_->datas_["EnemyState"].items["BerserkFireInterval"].value = dynamic_cast<EnemyBerserkState*>(currentState_.get())->fireInterval;
		stateIndex = 2;
	}


	ImGui::Separator();


	ImGui::DragFloat3("スケール", &scale_.x, 0.1f);
	global_->datas_["EnemyState"].items["Scale"].value = scale_;
	ImGui::DragFloat3("平行移動", &translate_.x, 0.1f);
	global_->datas_["EnemyState"].items["Translate"].value = translate_;

	model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, translate_);

	// 体力
	ImGui::SliderInt("体力", &hp_, 0, maxHp_);
	ImGui::SliderInt("最大体力", &maxHp_, 1, 10000);
	global_->datas_["EnemyState"].items["MaxHp"].value = maxHp_;
	if (maxHp_ < hp_) {
		hp_ = maxHp_;
	}
	
	ImGui::SliderInt("ダウン回数上限", &maxDownCount_, 1, 100);
	global_->datas_["EnemyState"].items["MaxDownCount"].value = maxDownCount_;

	if (ImGui::Button("Save")) {
		global_->SaveFile("EnemyState");
		std::string message = std::format("{}.json saved", "EnemyState");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}

	ImGui::End();

#endif // _DEBUG

}

void Enemy::ChangeState(std::unique_ptr<EnemyState> newState)
{
	if (currentState_) {
		currentState_->Exit(this);
	}

	currentState_ = std::move(newState);
	currentState_->Enter(this);
}

void Enemy::OnCollision(int damege)
{
	if(!dynamic_cast<EnemyDownState*>(currentState_.get())) {
		downCount_++;
	}

	hp_ -= damege;
	if (hp_ < 0) {
		hp_ = 0;
	}
}
