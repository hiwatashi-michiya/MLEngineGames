#include "EnemyHand.h"

#include <corecrt_math_defines.h>
#include <format>

#include"Externals/imgui/imgui.h"

void EnemyHand::Initialize(MLEngine::Object::Transform* parentTransform, bool isLeft)
{

#ifdef CLIENT_BUILD
	enemytype_ = "Enemy2";

#else
	enemytype_ = "Enemy1";

#endif

	isLeft_ = isLeft;

	global_ = GlobalVariables::GetInstance();

	global_->CreateGroup("Enemy1Hand");
	global_->CreateGroup("Enemy2Hand");

	global_->AddItem(enemytype_ + "Hand", "NormalSwingAngle", 0.0f);
	global_->AddItem(enemytype_ + "Hand", "AngrySwingAngle", 0.0f);
	global_->AddItem(enemytype_ + "Hand", "SwingSpeed", 1.0f);
	global_->AddItem(enemytype_ + "Hand", "ArmAngleAxis", MLEngine::Math::Vector3{ 0.0f, 0.0f, (float)M_PI });
	global_->AddItem(enemytype_ + "Hand", "AttackLeftJointPoint", MLEngine::Math::Vector3{ -1.5f, 2.0f, 0.0f });
	global_->AddItem(enemytype_ + "Hand", "LeftHandPosition", MLEngine::Math::Vector3{ -1.5f, 0.0f, 0.0f });
	global_->AddItem(enemytype_ + "Hand", "LeftHandScale", MLEngine::Math::Vector3{ 1.0f, 1.0f, 1.0f });
	global_->AddItem(enemytype_ + "Hand", "AttackRightJointPoint", MLEngine::Math::Vector3{ 1.5f, 2.0f, 0.0f });
	global_->AddItem(enemytype_ + "Hand", "RightHandPosition", MLEngine::Math::Vector3{ 1.5f, 0.0f, 0.0f });
	global_->AddItem(enemytype_ + "Hand", "RightHandScale", MLEngine::Math::Vector3{ 1.0f, 1.0f, 1.0f });

	
	global_->AddItem(enemytype_ + "Hand", "Amplitude", 0.5f);
	global_->AddItem(enemytype_ + "Hand", "SwingUpTargetTime", 0.5f);
	global_->AddItem(enemytype_ + "Hand", "SwingDownTargetTime", 0.5f);
	global_->AddItem(enemytype_ + "Hand", "AttackOmen", 1.0f);
	global_->AddItem(enemytype_ + "Hand", "SwingUp", 0.5f);
	global_->AddItem(enemytype_ + "Hand", "SwingDown", -1.0f);
	global_->AddItem(enemytype_ + "Hand", "MoveAfter", 0.25f);

	// 腕振り用のパラメーター取得
	normalSwingAngle_ = global_->GetFloatValue(enemytype_ + "Hand", "NormalSwingAngle");
	angrySwingAngle_ = global_->GetFloatValue(enemytype_ + "Hand", "AngrySwingAngle");
	swingSpeed_ = global_->GetFloatValue(enemytype_ + "Hand", "SwingSpeed");

	 swingUpTime_ = { 0.0f, global_->GetFloatValue(enemytype_ + "Hand", "SwingUpTargetTime")};
	 swingDownTime_ = { 0.0f, global_->GetFloatValue(enemytype_ + "Hand", "SwingDownTargetTime")};
	 afterTime_ = { 0.0f, global_->GetFloatValue(enemytype_ + "Hand", "MoveAfter") };
	 amplitude_ = global_->GetFloatValue(enemytype_ + "Hand", "Amplitude");
	 swingUp_ = global_->GetFloatValue(enemytype_ + "Hand", "SwingUp");
	 swingDown_ = global_->GetFloatValue(enemytype_ + "Hand", "SwingDown");

	jointTransform_ = std::make_unique<MLEngine::Object::Transform>();
	jointTransform_->SetParent(parentTransform);
	transform_ = std::make_unique<MLEngine::Object::Transform>();
	transform_->SetParent(jointTransform_.get());



#ifdef CLIENT_BUILD

	armAngleAxis_ = global_->GetVector3Value(enemytype_ + "Hand", "ArmAngleAxis");
	if (isLeft) {
		frontTexture_ = "./Resources/Texture/enemy2_lefthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy2_righthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy2_lefthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy2_righthand_attack.png";


		jointTransform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "LeftJointPoint");
		jointTransform_->rotateQuaternion = MLEngine::Math::ConvertFromEuler(armAngleAxis_);

		transform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "LeftHandPosition");
		transform_->scale = global_->GetVector3Value(enemytype_ + "Hand", "LeftHandScale");

	}
	else {
		frontTexture_ = "./Resources/Texture/enemy2_righthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy2_lefthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy2_righthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy2_lefthand_attack.png";

		jointTransform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "RightJointPoint");
		jointTransform_->rotateQuaternion = MLEngine::Math::ConvertFromEuler(armAngleAxis_);

		transform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "RightHandPosition");
		transform_->scale = global_->GetVector3Value(enemytype_ + "Hand", "RightHandScale");
	}

	angle_ = 180.0f;
	
#else
	if (isLeft) {
		frontTexture_ = "./Resources/Texture/enemy1_lefthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy1_righthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy1_lefthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy1_righthand_attack.png";

		jointTransform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "LeftJointPoint");
		transform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "LeftHandPosition");
		transform_->scale = global_->GetVector3Value(enemytype_ + "Hand", "LeftHandScale");
	}
	else {
		frontTexture_ = "./Resources/Texture/enemy1_righthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy1_lefthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy1_righthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy1_lefthand_attack.png";

		jointTransform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "RightJointPoint");
		transform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "RightHandPosition");
		transform_->scale = global_->GetVector3Value(enemytype_ + "Hand", "RightHandScale");
	}

	
#endif

	startPosition_ = transform_->translate;

	frontPlane_.Initialize(frontTexture_, 1);
	frontPlane_.transform.translate = { 0.0f, 0.0f, -0.001f };
	frontPlane_.transform.scale = transform_->scale;
	frontPlane_.transform.SetParent(jointTransform_.get());
	

	backPlane_.Initialize(backTexture_, 1);
	backPlane_.transform.rotateQuaternion = MLEngine::Math::ConvertFromEuler({ 0.0f, 180.0f * (float)(M_PI / 180.0f), 0.0f });
	backPlane_.transform.translate = { 0.0f, 0.0f, 0.001f };
	backPlane_.transform.scale = transform_->scale;
	backPlane_.transform.SetParent(jointTransform_.get());

	joint_.Initialize("./Resources/white.png", 1);
	joint_.transform.scale = { 0.1f,0.1f,0.1f };
	joint_.color = { 1.0f, 0.5f, 0.0f, 1.0f };

	
	
}

void EnemyHand::Update()
{


#ifdef CLIENT_BUILD

	

	switch (handState_)
	{
	case EnemyHand::HandState::kNone:
		angle_ = 180.0f;
		break;

	case EnemyHand::HandState::kNormal:

		if (isLeft_) {
			angle_ += swingSpeed_;
			if (angle_ > normalSwingAngle_ + 180.0f|| angle_ < 180.0f) {
				angle_ = std::clamp(angle_, 180.0f, normalSwingAngle_ + 180.0f);
				swingSpeed_ *= -1.0f;
			}

		}
		else {
			angle_ -= swingSpeed_;
			if (angle_ < -normalSwingAngle_ + 180.0f|| angle_ >  180.0f) {
				angle_ = std::clamp(angle_, -normalSwingAngle_, 180.0f);
				swingSpeed_ *= -1.0f;
			}
		}


		break;
	case EnemyHand::HandState::kAngry:
		if (isLeft_) {
			angle_ += swingSpeed_;
			if (angle_ > angrySwingAngle_ + 180.0f || angle_ < 180.0f) {
				angle_ = std::clamp(angle_, 180.0f, angrySwingAngle_);
				swingSpeed_ *= -1.0f;
			}

		}
		else {
			angle_ -= swingSpeed_;
			if (angle_ < -angrySwingAngle_ + 180.0f || angle_ >  180.0f) {
				angle_ = std::clamp(angle_, -angrySwingAngle_, 180.0f);
				swingSpeed_ *= -1.0f;
			}
		}

		break;
	case EnemyHand::HandState::kAttack:
		angle_ = 180.0f;

		if (swingUpTime_.elapsedTime_ < swingUpTime_.targetTime_) {
			swingUpTime_.elapsedTime_ += 1.0f / 60.0f;
			float t = swingUpTime_.elapsedTime_ / swingUpTime_.targetTime_;
			transform_->translate.y = -swingUp_ * t;

		}
		else if (swingDownTime_.elapsedTime_ < swingDownTime_.targetTime_) {
			swingDownTime_.elapsedTime_ += 1.0f / 60.0f;
			float t = swingDownTime_.elapsedTime_ / swingDownTime_.targetTime_;
			transform_->translate.y = -swingUp_ + (-swingDown_ * t);
		}
		else if (afterTime_.elapsedTime_ < afterTime_.targetTime_) {
			afterTime_.elapsedTime_ += 1.0f / 60.0f;
		}
		else {
			handState_ = HandState::kNone;
			transform_->translate = startPosition_;
		}


		break;
	}


	
#else
	

	

	switch (handState_)
	{
	case EnemyHand::HandState::kNone:
		angle_ = 0.0f;
		break;

	case EnemyHand::HandState::kNormal:

		if (isLeft_) {
			angle_ += swingSpeed_;
			if (angle_ > normalSwingAngle_ || angle_ < 0.0f) {
				angle_ = std::clamp(angle_, 0.0f, normalSwingAngle_);
				swingSpeed_ *= -1.0f;
			}
			
		}
		else {
			angle_ -= swingSpeed_;
			if (angle_ < -normalSwingAngle_ || angle_ > 0.0f) {
				angle_ = std::clamp(angle_, -normalSwingAngle_, 0.0f);
				swingSpeed_ *= -1.0f;
			}
		}

		
		break;
	case EnemyHand::HandState::kAngry:
		if (isLeft_) {
			angle_ += swingSpeed_;
			if (angle_ > angrySwingAngle_ || angle_ < 0.0f) {
				angle_ = std::clamp(angle_, 0.0f, angrySwingAngle_);
				swingSpeed_ *= -1.0f;
			}

		}
		else {
			angle_ -= swingSpeed_;
			if (angle_ < -angrySwingAngle_ || angle_ > 0.0f) {
				angle_ = std::clamp(angle_, -angrySwingAngle_, 0.0f);
				swingSpeed_ *= -1.0f;
			}
		}

		break;
	case EnemyHand::HandState::kAttack:
		angle_ = 0.0f;

		if(swingUpTime_.elapsedTime_ < swingUpTime_.targetTime_){
			swingUpTime_.elapsedTime_ += 1.0f / 60.0f;
			float t = swingUpTime_.elapsedTime_ / swingUpTime_.targetTime_;
			transform_->translate.y = swingUp_ * t;
			
		}
		else if(swingDownTime_.elapsedTime_ < swingDownTime_.targetTime_){
			swingDownTime_.elapsedTime_ += 1.0f / 60.0f;
			float t = swingDownTime_.elapsedTime_ / swingDownTime_.targetTime_;
			transform_->translate.y = swingUp_ + (swingDown_ * t);
		}
		else if(afterTime_.elapsedTime_ < afterTime_.targetTime_){
			afterTime_.elapsedTime_ += 1.0f / 60.0f;
		}
		else{
			handState_ = HandState::kNone;
			transform_->translate = startPosition_;
		}


		break;
	}

#endif

	jointTransform_->rotateQuaternion = MLEngine::Math::ConvertFromEuler({ 0.0f, 0.0f, angle_ * (float)(M_PI / 180.0f) });
	

	jointTransform_->UpdateMatrix();
	transform_->UpdateMatrix();
	frontPlane_.transform.translate = transform_->translate;
	backPlane_.transform.translate = transform_->translate;


}

void EnemyHand::DebugUI(std::string uiname, std::string dir)
{
#ifdef _DEBUG

	ImGui::Begin(uiname.c_str());

	ImGui::DragFloat3("原点", &jointTransform_->translate.x, 0.1f);
	global_->datas_[enemytype_ + "Hand"].items[dir + "JointPoint"].value = jointTransform_->translate;
	ImGui::DragFloat3("手の位置", &transform_->translate.x, 0.1f);
	global_->datas_[enemytype_ + "Hand"].items[dir + "HandPosition"].value = transform_->translate;
	ImGui::DragFloat3("手のサイズ", &transform_->scale.x, 0.1f);
	frontPlane_.transform.scale = transform_->scale;
	backPlane_.transform.scale = transform_->scale;
	global_->datas_[enemytype_ + "Hand"].items[dir + "HandScale"].value = transform_->scale;

	ImGui::Separator();

	ImGui::SliderFloat("振り幅(通常時)", &normalSwingAngle_, 0.0f, 90.0f);
	global_->datas_[enemytype_ + "Hand"].items["NormalSwingAngle"].value = normalSwingAngle_;
	ImGui::SliderFloat("振り幅(怒り時)", &angrySwingAngle_, 0.0f, 90.0f);
	global_->datas_[enemytype_ + "Hand"].items["AngrySwingAngle"].value = angrySwingAngle_;
	ImGui::DragFloat("振り速度", &swingSpeed_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["SwingSpeed"].value = swingSpeed_;

	ImGui::Separator();

	ImGui::DragFloat("振り上げ時間", &swingUpTime_.targetTime_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["SwingUpTargetTime"].value = swingUpTime_.targetTime_;
	ImGui::DragFloat("振り上げ量", &swingUp_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["SwingUp"].value = swingUp_;
	ImGui::DragFloat("振り下げ時間", &swingDownTime_.targetTime_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["SwingDownTargetTime"].value = swingDownTime_.targetTime_;
	ImGui::DragFloat("振り下げ量", &swingDown_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["SwingDown"].value = swingDown_;
	ImGui::DragFloat("振り後移動時間", &afterTime_.targetTime_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["MoveAfter"].value = afterTime_.targetTime_;
	ImGui::DragFloat("振幅", &amplitude_, 0.01f);
	global_->datas_[enemytype_ + "Hand"].items["Amplitude"].value = amplitude_;

	if (ImGui::Button("Save")) {
		global_->SaveFile(enemytype_ + "Hand");
		std::string message = std::format("{}.json saved", enemytype_ + "Hand");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}

	ImGui::End();

	joint_.transform.translate = jointTransform_->translate;
#endif // _DEBUG
}

void EnemyHand::SetHandState(HandState state)
{
	handState_ = state;
	if(state != HandState::kAttack){
		frontPlane_.Initialize(frontTexture_, 1);
		backPlane_.Initialize(backTexture_, 1);

		if (isLeft_) {
			transform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "LeftHandPosition");
			transform_->scale = global_->GetVector3Value(enemytype_ + "Hand", "LeftHandScale");
		}
		else {
			transform_->translate = global_->GetVector3Value(enemytype_ + "Hand", "RightHandPosition");
			transform_->scale = global_->GetVector3Value(enemytype_ + "Hand", "RightHandScale");
		}

		
	}
	else {
		swingUpTime_.elapsedTime_ = 0.0f;
		swingDownTime_.elapsedTime_ = 0.0f;
		afterTime_.elapsedTime_ = 0.0f;

		frontPlane_.Initialize(frontFistTexture_, 1);
		backPlane_.Initialize(backFistTexture_, 1);
	}
}
