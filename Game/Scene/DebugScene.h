#pragma once
#include "BaseScene.h"
#include "RigidModel.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Audio.h"
#include "VirtualController.h"
#include "Particle3D.h"
#include <memory>

#include "Collider.h"
#include "Transform.h"

#include "../Joycon/Joycon.h"

/// <summary>
/// デバッグシーン、エンジン機能理解用
/// シーンはmain.cppで初期シーンのポインタを渡す
/// </summary>
class DebugScene : public BaseScene
{
public:
	DebugScene();
	~DebugScene();

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	void DrawImgui();

private:

	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;
	VirtualController* vController_ = nullptr;

	//
	MLEngine::Object::DebugCamera debugCamera_;
	//オーディオ
	MLEngine::Resource::Audio se1_;
	//通常モデル
	MLEngine::Resource::Sprite3D sprite3D_;
	MLEngine::Resource::RigidModel model_;
	MLEngine::Resource::RigidModel model2_;
	MLEngine::Resource::RigidModel model3_;
	//トランスフォーム
	MLEngine::Object::Transform transform2_;
	MLEngine::Object::Transform transform3_;
	//Vector3宣言
	MLEngine::Math::Vector3 position_;
	//Matrix4x4宣言
	MLEngine::Math::Matrix4x4 matrix_;
	//パーティクル
	std::unique_ptr<MLEngine::Resource::Particle3D> particle_;
	//スプライト
	std::unique_ptr<MLEngine::Resource::Sprite2D> sprite_;
	//コライダーテスト
	MLEngine::Object::Collision::SphereCollider sphere_;
	MLEngine::Resource::LineSphere lineSphere_;
	MLEngine::Object::Collision::BoxCollider box_;
	MLEngine::Resource::LineBox lineBox_;
	MLEngine::Resource::Texture tex_;

	bool showBox_ = true;
	bool showSphere_ = true;
	bool isDebugCamera_ = false;

	std::unique_ptr<Joycon> joyconInput;
	Vector3 test;
	Quaternion modelRot_;
};


