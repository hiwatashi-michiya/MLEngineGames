#include "Enemy.h"

#include <format>

#include "Input/Input.h"
#include"Externals/imgui/imgui.h"

void Enemy::Initialize()
{
	global_ = GlobalVariables::GetInstance();

	global_->SetValue("EnemyState", "Scale", scale_);
	scale_ = global_->GetVector3Value("EnemyState", "Scale");
	global_->SetValue("EnemyState", "Translate", translate_);
	translate_ = global_->GetVector3Value("EnemyState", "Translate");
	global_->SetValue("EnemyState", "MaxHp", maxHp_);
	maxHp_ = global_->GetIntValue("EnemyState", "MaxHp");
	hp_ = global_->GetIntValue("EnemyState", "MaxHp");

	//position_ = initialPosition_;

	model_.Initialize("./Resources/model/plane/plane.obj");
	model_.worldMatrix = MLEngine::Math::MakeAffineMatrix(scale_, { 0.0f, 0.0f, 0.0f, 1.0f }, translate_);

	/*MLEngine::Resource::Texture texture;
	texture.Load("./Resources/EngineResources/defaultMask.png");
	sprite_.reset(MLEngine::Resource::Sprite::Create(texture, position_, { 1.0f,1.0f,1.0f,1.0f }));
	sprite_->anchorPoint = { 0.5f,0.5f };
	sprite_->size = { 320.0f,160.0f };*/


	ChangeState(std::make_unique<EnemyNormalState>());

	bulletManager_ = std::make_unique<BulletManager>();
	bulletManager_->Initialize();
	//bulletManager_->SetLaunchPosition(LaunchPosition());

	for (int i = 0; i < 3; ++i) {
		//bulletManager_->SpawnBullet({ LaunchPosition().x + distance_ * (i - 1), LaunchPosition().y}, {(i - 1) * 0.8f , 1.0f}, 4.0f);
	}

	hp_ = maxHp_;

}

void Enemy::Update()
{
	if (!dynamic_cast<EnemyBerserkState*>(currentState_.get())) {
		if(hp_ <= maxHp_ * 0.3f) {
			ChangeState(std::make_unique<EnemyBerserkState>());
		}
	}

	MLEngine::Input::Manager* input = MLEngine::Input::Manager::GetInstance();

	if (input->GetKeyboard()->Trigger(DIK_H)) {
		hp_ = maxHp_ * 0.3f;
	}
	if (input->GetKeyboard()->Trigger(DIK_J)) {
		ChangeState(std::make_unique<EnemyDownState>());
	}

	currentState_->Update(this);
	bulletManager_->Update();
	//bulletManager_->SetLaunchPosition(LaunchPosition());




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
	if (dynamic_cast<EnemyNormalState*>(currentState_.get())) {
		ImGui::DragFloat("弾速度", &dynamic_cast<EnemyNormalState*>(currentState_.get())->bulletSpeed_, 0.1f);
		ImGui::DragFloat("発射間隔", &dynamic_cast<EnemyNormalState*>(currentState_.get())->fireInterval, 0.1f);
		ImGui::Text("経過時間 : %f" ,dynamic_cast<EnemyNormalState*>(currentState_.get())->intervalTime_);
		global_->datas_["EnemyState"].items["NormalBulletSpeed"].value = dynamic_cast<EnemyNormalState*>(currentState_.get())->bulletSpeed_;
		global_->datas_["EnemyState"].items["NormalFireInterval"].value = dynamic_cast<EnemyNormalState*>(currentState_.get())->fireInterval;
		stateIndex = 0;
	}
	else if (dynamic_cast<EnemyDownState*>(currentState_.get())) {
		ImGui::DragFloat("ダウン時間", &dynamic_cast<EnemyDownState*>(currentState_.get())->downTime, 0.1f);
		ImGui::Text("経過時間 : %f", dynamic_cast<EnemyDownState*>(currentState_.get())->elapsedTime_);
		global_->datas_["EnemyState"].items["DownTime"].value = dynamic_cast<EnemyDownState*>(currentState_.get())->downTime;
		stateIndex = 1;
	}
	else if (dynamic_cast<EnemyBerserkState*>(currentState_.get())) {
		ImGui::DragFloat("弾速度", &dynamic_cast<EnemyBerserkState*>(currentState_.get())->bulletSpeed_, 0.1f);
		ImGui::DragFloat("発射間隔", &dynamic_cast<EnemyBerserkState*>(currentState_.get())->fireInterval, 0.1f);
		ImGui::Text("経過時間 : %f", dynamic_cast<EnemyBerserkState*>(currentState_.get())->intervalTime_);
		global_->datas_["EnemyState"].items["BerserkBulletSpeed"].value = dynamic_cast<EnemyBerserkState*>(currentState_.get())->bulletSpeed_;
		global_->datas_["EnemyState"].items["BerserkFireInterval"].value = dynamic_cast<EnemyBerserkState*>(currentState_.get())->fireInterval;
		stateIndex = 2;
	}


	ImGui::Separator();

	// 位置、サイズ
	/*ImGui::DragFloat2("位置", &sprite_->position.x, 1.0f);
	ImGui::DragFloat2("サイズ", &sprite_->size.x, 1.0f);*/

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

	if (ImGui::Button("Save")) {
		global_->SaveFile("EnemyState");
		std::string message = std::format("{}.json saved", "EnemyState");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}

	ImGui::End();

#endif // _DEBUG

}

void Enemy::Draw(MLEngine::Object::Camera* camera)
{
}

void Enemy::ChangeState(std::unique_ptr<EnemyState> newState)
{
	if (currentState_) {
		currentState_->Exit(this);
	}

	currentState_ = std::move(newState);
	currentState_->Enter(this);
}

//MLEngine::Math::Vector2 Enemy::LaunchPosition()
//{
//	/*MLEngine::Math::Vector2 launchPosition = position_;
//	launchPosition.y += sprite_->size.y / 2.0f;
//	return launchPosition;*/
//}
