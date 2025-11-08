#include "RigidModel.h"
#include <cassert>
#include "Engine/Convert.h"
#include "Engine/Core/Render/ShaderManager.h"
#include <fstream>
#include <sstream>
#include "Core/ImGuiManager.h"
#include "Buffer/BufferResource.h"
#include "ModelManager.h"
#include "InstancingModel.h"
#include "../ResourceManager.h"

#pragma comment(lib, "dxcompiler.lib")

using namespace MLEngine::Resource;
using namespace MLEngine::Core::Render;
using namespace MLEngine::Math;
using namespace MLEngine::Object;

RigidModel::~RigidModel()
{
	Resource::Manager::GetInstance()->RemoveRigidModel(this);
}

void RigidModel::Initialize(const std::string& filename, [[maybe_unused]] const std::string& texturename) {

	localMatrix = Matrix4x4::Identity();
	worldMatrix = Matrix4x4::Identity();
	worldViewProjectionMatrix = Matrix4x4::Identity();
	color = { 1.0f,1.0f,1.0f,1.0f };
	//初期化時に描画用のリストに登録
	Resource::Manager::GetInstance()->AddRigidModel(this);
	//既にインスタンシング用のモデルを作成している場合、それを返す
	if (Model::Manager::GetInstance()->IsExistModel(filename)) {
		instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);
		return;
	}
	//新規でインスタンシング用のモデルを作成
	Model::Manager::GetInstance()->AddModel(filename);
	//ポインタを渡す
	instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);

}

void RigidModel::LoadAnimation(const std::string& filename) {

	//インスタンス生成
	if (!animation_) {

		animation_ = std::make_unique<Animation>();

	}

	*animation_ = LoadAnimationFile(filename);

	if (!skeleton_) {

		skeleton_ = std::make_unique<Skeleton>();

	}

	*skeleton_ = CreateSkeleton(instancingModel_->mesh_->GetModelData().rootNode);

}

void RigidModel::ResetAnimation() {

	//アニメーションが存在している時のみリセットを行う
	if (animation_ and animation_->nodeAnimations.size() != 0) {

		animationTime_ = 0.0f;
		NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[instancingModel_->mesh_->GetModelData().rootNode.name]; //rootNodeのanimationを取得
		Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
		Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		localMatrix = MakeAffineMatrix(scale, rotate, translate);

	}

}

void RigidModel::UpdateAnimation() {

	isEndAnimation_ = false;

	//アニメーションが存在していて、再生フラグが立っている時
	if (animation_ and isStartAnimation_ and animation_->nodeAnimations.size() != 0) {

		//現在のアニメーションタイムをアニメーション速度分加算
		animationTime_ += animationSpeed_ / 60.0f;

		//アニメーションタイムが全体の尺を超えていたら終点とみなす
		if (animationTime_ >= animation_->duration) {
			
			animationTime_ = animation_->duration;

			//ループしなければフラグを降ろす
			if (!isLoop_) {
				isStartAnimation_ = false;
			}

			isEndAnimation_ = true;
		}

		//アニメーションの時間調整
		animationTime_ = std::fmod(animationTime_, animation_->duration);
		
		if (skeleton_) {

			ApplyAnimation(*skeleton_, *animation_, animationTime_);
			UpdateSkeleton(*skeleton_);

		}

		NodeAnimation& rootNodeAnimation = animation_->nodeAnimations[instancingModel_->mesh_->GetModelData().rootNode.name]; //rootNodeのanimationを取得
		Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime_);
		Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime_);
		Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime_);
		localMatrix = MakeAffineMatrix(scale, rotate, translate);

	}

}

void RigidModel::Regist()
{

	if (instancingModel_) {
		instancingModel_->Regist(this);
	}

}

void RigidModel::SetMesh(const std::string& filename, [[maybe_unused]] const std::string& texturename)
{

	//既にインスタンシング用のモデルを作成している場合、それを返す
	if (Model::Manager::GetInstance()->IsExistModel(filename)) {
		instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);
		return;
	}
	//新規でインスタンシング用のモデルを作成
	Model::Manager::GetInstance()->AddModel(filename);
	//ポインタを渡す
	instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);

}

