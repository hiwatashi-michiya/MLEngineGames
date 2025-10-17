#pragma once
#include "Scene/BaseScene.h"
#include "Model/Model.h"
#include "Object/Camera.h"
#include "Sprite/Sprite.h"
#include "Audio/Audio.h"

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

	MLEngine::Object::Camera camera_;

	MLEngine::Resource::Audio se1_;

	MLEngine::Resource::RigidModel model_;
	//Vector3宣言
	MLEngine::Math::Vector3 position_;
	//Matrix4x4宣言
	MLEngine::Math::Matrix4x4 matrix_;
	

};


