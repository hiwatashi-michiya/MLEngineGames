#pragma once
#include "BaseScene.h"
#include <memory>

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

		private:

			//現在のシーン
			std::unique_ptr<BaseScene> scene_;
			//次のシーン
			std::unique_ptr<BaseScene> nextScene_;

		private:
			//シングルトン化
			Manager() = default;
			~Manager() = default;
			Manager(const Manager&) = delete;
			const Manager& operator=(const Manager&) = delete;

		};

	}

}

