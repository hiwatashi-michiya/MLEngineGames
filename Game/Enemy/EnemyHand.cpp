#include "EnemyHand.h"

#include <corecrt_math_defines.h>
#include <format>

#include"Externals/imgui/imgui.h"

void EnemyHand::Initialize(MLEngine::Object::Transform* parentTransform, bool isLeft)
{

	isLeft_ = isLeft;

	global_ = GlobalVariables::GetInstance();

	jointTransform_ = std::make_unique<MLEngine::Object::Transform>();
	jointTransform_->SetParent(parentTransform);
	transform_ = std::make_unique<MLEngine::Object::Transform>();
	transform_->SetParent(jointTransform_.get());

#ifdef CLIENT_BUILD
	if (isLeft) {
		frontTexture_ = "./Resources/Texture/enemy2_lefthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy2_righthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy2_lefthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy2_righthand_attack.png";
	}
	else {
		frontTexture_ = "./Resources/Texture/enemy2_angry.png";
		backTexture_ = "./Resources/Texture/enemy2_attack.png";
		frontFistTexture_ = "./Resources/Texture/enemy2_righthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy2_lefthand_attack.png";
	}
#else
	if (isLeft) {
		frontTexture_ = "./Resources/Texture/enemy1_lefthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy1_righthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy1_lefthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy1_righthand_attack.png";

		jointTransform_->translate = global_->GetVector3Value("EnemyHand", "LeftJointPoint");
		transform_->translate = global_->GetVector3Value("EnemyHand", "LeftHandPosition");
		transform_->scale = global_->GetVector3Value("EnemyHand", "LeftHandScale");
	}
	else {
		frontTexture_ = "./Resources/Texture/enemy1_righthand_normal.png";
		backTexture_ = "./Resources/Texture/enemy1_lefthand_normal.png";
		frontFistTexture_ = "./Resources/Texture/enemy1_righthand_attack.png";
		backFistTexture_ = "./Resources/Texture/enemy1_lefthand_attack.png";

		jointTransform_->translate = global_->GetVector3Value("EnemyHand", "RightJointPoint");
		transform_->translate = global_->GetVector3Value("EnemyHand", "RightHandPosition");
		transform_->scale = global_->GetVector3Value("EnemyHand", "RightHandScale");
	}

	
#endif

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
	
#else
	

	

	switch (handState_)
	{
	case EnemyHand::HandState::kNone:
		angle_ = 0.0f;
		break;

	case EnemyHand::HandState::kNormal:

		if (isLeft_) {
			angle_ += swingSpeed_;
			if (angle_ > swingAngle_ || angle_ < 0.0f) {
				angle_ = std::clamp(angle_, 0.0f, swingAngle_);
				swingSpeed_ *= -1.0f;
			}
			
		}
		else {
			angle_ -= swingSpeed_;
			if (angle_ < -swingAngle_ || angle_ > 0.0f) {
				angle_ = std::clamp(angle_, -swingAngle_, 0.0f);
				swingSpeed_ *= -1.0f;
			}
		}

		
		break;
	case EnemyHand::HandState::kAngry:
		break;
	case EnemyHand::HandState::kAttack:
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
	global_->datas_["EnemyHand"].items[dir + "JointPoint"].value = jointTransform_->translate;
	ImGui::DragFloat3("手の位置", &transform_->translate.x, 0.1f);
	global_->datas_["EnemyHand"].items[dir + "HandPosition"].value = transform_->translate;
	ImGui::DragFloat3("手のサイズ", &transform_->scale.x, 0.1f);
	frontPlane_.transform.scale = transform_->scale;
	backPlane_.transform.scale = transform_->scale;
	global_->datas_["EnemyHand"].items[dir + "HandScale"].value = transform_->scale;

	ImGui::Separator();

	ImGui::SliderFloat("振り幅", &swingAngle_, 0.0f, 90.0f);
	ImGui::DragFloat("振り速度", &swingSpeed_, 0.01f);


	if (ImGui::Button("Save")) {
		global_->SaveFile("EnemyHand");
		std::string message = std::format("{}.json saved", "EnemyHand");
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
	}

	ImGui::End();

	joint_.transform.translate = jointTransform_->translate;
#endif // _DEBUG
}