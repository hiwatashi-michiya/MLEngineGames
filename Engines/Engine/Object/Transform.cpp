#include "Transform.h"
#include "ImguiManager.h"


using namespace MLEngine::Math;
using namespace MLEngine::Object;

Transform::Transform()
{
	//値をデフォルトに設定
	scale = Vector3::Identity();
	rotate = { 0.0f,0.0f,0.0f };
	rotateQuaternion = rotateQuaternion.IdentityQuaternion();
	translate = { 0.0f,0.0f,0.0f };

	worldMatrix = MakeAffineMatrix(scale, rotate, translate);

}

Transform::~Transform()
{
}

void Transform::UpdateMatrix() {
	//クォータニオン正規化
	rotateQuaternion.Normalize();
	//行列作成、更新
	worldMatrix = MakeAffineMatrix(scale, rotateQuaternion, translate);
	//親がいたら親行列も掛ける
	if (parent_) {
		worldMatrix = worldMatrix * parent_->worldMatrix;
	}

}

void Transform::Debug() {

#ifdef _DEBUG
	ImGui::DragFloat3("translate", &translate.x, 0.1f);

	if (ImGui::DragFloat3("rotate", &rotate.x, 0.05f)) {
		rotateQuaternion = ConvertFromEuler(rotate);
	}

	ImGui::DragFloat3("scale", &scale.x, 0.1f);
#endif // _DEBUG

}
