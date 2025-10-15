#pragma once
#include "Model/Model.h"
#include "Model/InstancingModel.h"
#include <array>
#include <unordered_map>

namespace MLEngine::Core::Render {

	namespace Model {

		/// <summary>
		/// モデルの描画管理などを行うクラス
		/// </summary>
		class Manager
		{
		public:
			//インスタンス取得
			static Manager* GetInstance();

			//ブレンドモード
			enum BlendMode {
				kNormal,//通常
				kAdd,//加算
				kSubtract,//減算
				kMultiply,//乗算
				kScreen,//スクリーン

				kCountBlend
			};
			/// <summary>
			/// 初期化
			/// </summary>
			void Initialize();

			/// <summary>
			/// 新規モデルを追加する
			/// </summary>
			void AddModel(const std::string& fileName);

			void PreDraw(ID3D12GraphicsCommandList* commandList);

			void PostDraw();

			/// <summary>
			/// 描画処理
			/// </summary>
			void Render();

			/// <summary>
			/// モデルを既に作成しているか
			/// </summary>
			/// <param name="fileName">確認するファイルの名前</param>
			/// <returns></returns>
			bool IsExistModel(const std::string& fileName) { return models_.find(fileName) != models_.end(); }

			MLEngine::Resource::InstancingModel* GetModel(const std::string& fileName);

		private:

			//コマンドリスト
			ID3D12GraphicsCommandList* commandList_;
			//ルートシグネチャ
			ID3D12RootSignature* rootSignature_;
			//PSO
			ID3D12PipelineState* pipelineStates_[BlendMode::kCountBlend];

			BlendMode currentBlendMode_;

			//モデルと名前を紐づけたマップ
			std::unordered_map<std::string, std::unique_ptr<MLEngine::Resource::InstancingModel>> models_;

		private:
			//シングルトン化
			Manager() = default;
			~Manager() = default;
			Manager(const Manager&) = delete;
			const Manager& operator=(const Manager&) = delete;

		};

	}


}



