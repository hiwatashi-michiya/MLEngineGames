#pragma once

namespace MLEngine::Scene {
	class Manager;
}

/// <summary>
/// シーンの基底クラス
/// </summary>
class BaseScene
{
public:
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

private:

	MLEngine::Scene::Manager* sceneManager_ = nullptr;

};
