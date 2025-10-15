#include "DebugCamera.h"
#include "ImGuiManager.h"

using namespace MLEngine::Object;
using namespace MLEngine;
using namespace MLEngine::Math;

DebugCamera::DebugCamera()
{
	camera_ = std::make_shared<Camera>();
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Initialize() {

	input_ = Input::Manager::GetInstance();

	camera_->Initialize();
	camera_->position_ = startPosition_;
	camera_->rotation_.x = startRotate_;

}

void DebugCamera::Update() {

	//マウスホイールをクリックした時
	if (input_->GetMouse()->PushMouse(Input::MouseButton::kCenter)) {

		//マウスの動きに合わせて回転
		if (input_->GetMouse()->GetMouseMove().x != 0.0f) {

			camera_->rotation_.y += input_->GetMouse()->GetMouseMove().x * rotateSensi_;

		}

		if (input_->GetMouse()->GetMouseMove().y != 0.0f) {

			camera_->rotation_.x += input_->GetMouse()->GetMouseMove().y * rotateSensi_;

		}

	}
	//右クリックした時
	else if (input_->GetMouse()->PushMouse(Input::MouseButton::kRight)) {

		//マウスの動きに合わせて平行移動
		if (input_->GetMouse()->GetMouseMove().x != 0.0f) {

			camera_->position_ -= camera_->matWorld_.GetXAxis() * input_->GetMouse()->GetMouseMove().x * moveSensi_;

		}

		if (input_->GetMouse()->GetMouseMove().y != 0.0f) {

			camera_->position_ -= camera_->matWorld_.GetYAxis() * input_->GetMouse()->GetMouseMove().y * moveSensi_;

		}

	}
	//マウスホイールを回した時
	if (input_->GetMouse()->GetMouseWheel() != 0.0f) {
		//カメラのZ軸を平行移動
		camera_->position_+= camera_->matWorld_.GetZAxis() * float(input_->GetMouse()->GetMouseWheel()) * moveSensi_;

	}

	//ここからカメラの更新
	Quaternion cameraQuaternion = IdentityQuaternion();

	cameraQuaternion = cameraQuaternion.ConvertFromEuler(camera_->rotation_);

	camera_->matRotate_ = MakeRotateMatrix(cameraQuaternion);

	camera_->Update();

}

void DebugCamera::Debug() {

#ifdef _DEBUG

	ImGui::Begin("Debug Camera");

	ImGui::End();

#endif // _DEBUG


}
