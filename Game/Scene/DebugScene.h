#pragma once
#include "BaseScene.h"
#include "Model.h"
#include "Camera.h"
#include "Sprite.h"
#include "Audio.h"
#include "Input.h"
#include "Particle3D.h"
#include <memory>
#include "Collider.h"

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

	
	//オーディオ
	MLEngine::Resource::Audio se1_;
	//通常モデル
	MLEngine::Resource::RigidModel model_;
	//Vector3宣言
	MLEngine::Math::Vector3 position_;
	//Matrix4x4宣言
	MLEngine::Math::Matrix4x4 matrix_;
	//パーティクル
	std::unique_ptr<MLEngine::Resource::Particle3D> particle_;
	//コライダーテスト
	MLEngine::Object::Collision::SphereCollider sphere_;
	MLEngine::Resource::LineSphere lineSphere_;
	MLEngine::Object::Collision::BoxCollider box_;
	MLEngine::Resource::LineBox lineBox_;

	std::unique_ptr<Joycon> joyconInput;
};


