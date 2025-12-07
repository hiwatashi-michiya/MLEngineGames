#include "Sprite3D.h"
#include <cassert>
#include "Engine/Convert.h"
#include "Engine/Core/Render/ShaderManager.h"
#include <fstream>
#include <sstream>
#include "Core/ImGuiManager.h"
#include "Buffer/BufferResource.h"
#include "ModelManager.h"
#include "../Model/InstancingModel.h"
#include "../ResourceManager.h"

#pragma comment(lib, "dxcompiler.lib")

using namespace MLEngine::Resource;
using namespace MLEngine::Core::Render;
using namespace MLEngine::Math;
using namespace MLEngine::Object;

Sprite3D::~Sprite3D()
{
	Resource::Manager::GetInstance()->RemoveSprite3D(this);
}

void Sprite3D::Initialize(const std::string& textureName, uint32_t divide) {

	//分割数が0ではいけない
	assert(divide != 0);

	divide_ = divide;

	std::string path = "./Resources/EngineResources/plane/plane.obj";

	localMatrix = Matrix4x4::Identity();
	worldViewProjectionMatrix = Matrix4x4::Identity();
	color = { 1.0f,1.0f,1.0f,1.0f };

	materialData.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData.enableLighting = false;
	materialData.enableNormalMap = true;
	materialData.enableToonshading = true;
	materialData.shininess = 50.0f;
	materialData.uvTransform = MakeIdentity4x4();

	//初期化時に描画用のリストに登録
	Resource::Manager::GetInstance()->AddSprite3D(this);
	//既にインスタンシング用のモデルを作成している場合、それを返す
	if (Model::Manager::GetInstance()->IsExistModel(path)) {
		instancingModel_ = Model::Manager::GetInstance()->GetModel(path);
		texture_.Load(textureName);
		SetSizeParameters();
		return;
	}
	//新規でインスタンシング用のモデルを作成
	Model::Manager::GetInstance()->AddModel(path);
	//ポインタを渡す
	instancingModel_ = Model::Manager::GetInstance()->GetModel(path);
	texture_.Load(textureName);
	SetSizeParameters();

}

void Sprite3D::UpdateAnimation() {

	//アニメーション中
	if (isStartAnimation_) {

		//時間を加算
		countTime_ += animationSpeed_;
		//切り替え時間に到達したら
		if (countTime_ >= animationTime_ / float(divide_)) {

			countTime_ = 0.0f;
			//一コマ分ずらす
			uvTranslate_.x += uvMoveSize_;

			//一ループ分ずれたら
			if (uvTranslate_.x >= 1.0f) {
				//元に戻す
				uvTranslate_.x = 0.0f;
			}

		}

	}

}

void Sprite3D::Regist()
{

	//アスペクト比を計算に入れるため、一度元のスケールを保存
	Vector3 tmpScale = transform.scale;

	transform.scale.x = transform.scale.x * aspect_;

	transform.UpdateMatrix();
	//元のスケールの値に戻す
	transform.scale = tmpScale;

	materialData.uvTransform = MakeScaleMatrix(Vector3(uvScale_.x, uvScale_.y, 1.0f)) *
		MakeTranslateMatrix(Vector3(uvTranslate_.x, uvTranslate_.y, 0.0f));

	if (instancingModel_) {
		instancingModel_->Regist(this);
	}

}

void Sprite3D::SetSizeParameters()
{
	//アスペクト比を設定
	aspect_ = float(texture_.GetResource()->GetDesc().Width) / float(texture_.GetResource()->GetDesc().Height) / float(divide_);
	//uvScaleを設定
	uvScale_.x = 1.0f / float(divide_);
	//移動量を設定
	uvMoveSize_ = 1.0f / float(divide_);


}

void Sprite3D::Debug() {

#ifdef _DEBUG

	transform.Debug();
	ImGui::DragFloat("animation time", &animationTime_, 0.05f);
	ImGui::Checkbox("start animation", &isStartAnimation_);

#endif // _DEBUG

}
