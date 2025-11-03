#pragma once
#include <memory>
#include "Camera.h"
#include "BaseSceneFactory.h"

class BaseScene;

namespace MLEngine {

	namespace Scene {

		/// <summary>
		/// シーンの切り替えや更新、描画処理を行うクラス
		/// </summary>
		class Manager
		{
		public:
			//インスタンス取得
			static Manager* GetInstance();
			//更新
			void Update();
			//終了処理
			void Finalize();
			//描画コマンドを積む
			void Draw();
			//実際の描画
			void Render();
			//シーン切り替え
			void ChangeScene(BaseScene* nextScene);
			//シーン切り替え(文字列)
			void ChangeScene(const std::string& sceneName);
			//メインカメラセット
			void SetMainCamera(MLEngine::Object::Camera* camera) { mainCamera_ = camera; }
			//シーンファクトリーセット
			void SetSceneFactory(BaseSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
			//メインカメラ取得
			MLEngine::Object::Camera* GetMainCamera() { return mainCamera_; }

		private:

			//現在のシーン
			std::unique_ptr<BaseScene> scene_;
			//次のシーン
			std::unique_ptr<BaseScene> nextScene_;
			//メインカメラのポインタ
			MLEngine::Object::Camera* mainCamera_ = nullptr;
			//シーンファクトリー
			BaseSceneFactory* sceneFactory_ = nullptr;

		private:
			//シングルトン化
			Manager() = default;
			~Manager() = default;
			Manager(const Manager&) = delete;
			const Manager& operator=(const Manager&) = delete;

		};

	}

}

