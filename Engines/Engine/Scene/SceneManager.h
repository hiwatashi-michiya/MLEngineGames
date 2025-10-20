#pragma once
#include <memory>
#include "Camera.h"

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
			//メインカメラセット
			void SetMainCamera(MLEngine::Object::Camera* camera) { mainCamera_ = camera; }
			//メインカメラ取得
			MLEngine::Object::Camera* GetMainCamera() { return mainCamera_; }

		private:

			//現在のシーン
			std::unique_ptr<BaseScene> scene_;
			//次のシーン
			std::unique_ptr<BaseScene> nextScene_;
			//メインカメラのポインタ
			MLEngine::Object::Camera* mainCamera_ = nullptr;

		private:
			//シングルトン化
			Manager() = default;
			~Manager() = default;
			Manager(const Manager&) = delete;
			const Manager& operator=(const Manager&) = delete;

		};

	}

}

