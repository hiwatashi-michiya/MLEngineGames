#pragma once
#include "Scene/BaseScene.h"
#include "Model/Model.h"
#include "Object/Camera.h"
#include "Sprite/Sprite.h"
#include "Audio/Audio.h"
#include "Input/Input.h"
#include "Particle/Particle3D.h"
#include <memory>

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

	//入力デバイス
	MLEngine::Input::Manager* input_ = nullptr;

	//カメラ
	MLEngine::Object::Camera camera_;
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


};


