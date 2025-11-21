#pragma once
#include "Camera.h"
#include "Lighting/DirectionalLight.h"
#include "SceneManager.h"
#include "RenderManager.h"

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene
{
public:
	//カメラをマネージャーにセット
	void SetCamera() {
		camera_.position_ = { 0.0f,0.0f,-20.0f };
		MLEngine::Core::Render::Manager::GetInstance()->SetCamera(&camera_);
		sceneManager_->SetMainCamera(&camera_);
	}
	//平行光源をマネージャーにセット
	void SetLight() {
		MLEngine::Core::Render::Manager::GetInstance()->SetLight(&dLight_);
	}
	//初期化
	virtual void Initialize() = 0;
	//終了処理
	virtual void Finalize() = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;

	virtual ~BaseScene() = default;
	//シーンマネージャーセット
	virtual void SetSceneManager(MLEngine::Scene::Manager* sceneManager) { sceneManager_ = sceneManager; }

protected:

	MLEngine::Scene::Manager* sceneManager_ = nullptr;

	//カメラ
	MLEngine::Object::Camera camera_;
	//平行光源
	MLEngine::Lighting::DirectionalLight dLight_;

};
