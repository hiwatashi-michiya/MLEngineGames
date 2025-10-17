#include "DebugScene.h"

using namespace MLEngine::Resource;

DebugScene::DebugScene()
{
}

DebugScene::~DebugScene()
{
}

inline void DebugScene::Initialize()
{
	//お試しプッシュ

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-10.0f };

	model_.Initialize("./Resources/EngineResources/testObjects/axis.obj");
	particle_.reset(Particle3D::Create("./Resources/model/plane/plane.obj", 32));

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{

	
	for (int32_t i = 0; i < 32; i++) {
		//ビルボードフラグ
		//particle_->isBillboard_ = true;
		//モデル一つ一つのアクティブフラグ
		particle_->isActive_[i] = true;
		//トランスフォーム
		particle_->transforms_[i].translate_ = { i * 0.1f, 0.0f,-1.0f };
		particle_->transforms_[i].scale_ = { 1.0f,1.0f,1.0f };
		particle_->transforms_[i].rotateQuaternion_ = MLEngine::Math::ConvertFromEuler((0.0f,0.0f,));
		//色
		particle_->colors_[i] = { 1.0f, i / 32.0f, 1.0f, 1.0f };
	}

	camera_.Update();

}

void DebugScene::Draw()
{

	model_.Draw(&camera_);

	particle_->Draw(&camera_);

}
