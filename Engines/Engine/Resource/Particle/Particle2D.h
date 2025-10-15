#pragma once
#include "Object/Camera.h"
#include "RenderingData.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <wrl.h>
#include "Object/Transform.h"
#include "Texture/Texture.h"
#include "InstancingResource.h"

namespace MLEngine::Resource {

	/// <summary>
	/// 2D空間上のパーティクル描画クラス
	/// </summary>
	class Particle2D
	{
	public:

		//静的初期化
		static void StaticInitialize(ID3D12Device* device);
		//GPUに使う用のパーティクルデータ
		struct Particle2DForGPU {
			MLEngine::Math::Matrix4x4 WVP;
			std::array<MLEngine::Math::Vector2, 4> texcoords;
			MLEngine::Math::Vector4 color;
			MLEngine::Math::Vector2 size;
		};
		/// <summary>
		/// 頂点データ構造体
		/// </summary>
		struct VertexData {
			MLEngine::Math::Vector4 position; //位置
			MLEngine::Math::Vector2 texcoord; //uv座標
		};
		/// <summary>
		/// 2Dマテリアルデータ
		/// </summary>
		struct Material2D {
			MLEngine::Math::Vector4 color;
			MLEngine::Math::Matrix4x4 uvTransform;
		};
		//生成
		static Particle2D* Create(const std::string& filename, uint32_t instanceCount = 1);
		//描画前処理
		static void PreDraw(ID3D12GraphicsCommandList* commandList);
		//描画後処理
		static void PostDraw();
		//終了処理
		static void Finalize();

	public:
		//初期化
		void Initialize(const std::string& filename, uint32_t instanceCount);
		//描画コマンド積む
		void Draw(MLEngine::Object::Camera* camera);
		//実際の描画
		void Render();
		//ImGui更新
		void ImGuiUpdate();
		//テクスチャセット
		void SetTexture(MLEngine::Resource::Texture texture) { texture_ = texture; }
		//テクスチャセット
		void SetTexture(const std::string& name);
		//テクスチャパス取得
		const std::string& GetTexturePath() { return texturePath_; }
		//インスタンスカウントセット
		void SetInstanceCount(uint32_t num) { instanceCount_ = num; }
		//パーティクルが1つでもアクティブ状態ならtrue
		bool IsAnyActive();

		//ビルボードを使うかどうか
		bool isBillboard_ = true;
		//トランスフォームリスト
		std::vector<MLEngine::Math::Vector2> positions_{};
		//色リスト
		std::vector<MLEngine::Math::Vector4> colors_{};
		//ワールド行列リスト
		std::vector<MLEngine::Math::Matrix4x4> worldMatrices_{};
		//速度リスト
		std::vector<MLEngine::Math::Vector3> velocities_{};
		//アクティブフラグリスト
		std::vector<bool> isActive_{};
		//生存時間リスト
		std::vector<int32_t> lifeTimes_{};

		//位置
		MLEngine::Math::Vector2 position_{};

		//サイズ
		MLEngine::Math::Vector2 size_{};

		//スプライト上の表示範囲
		MLEngine::Math::Vector2 viewRect_{};

		//アンカーポイント
		MLEngine::Math::Vector2 anchorPoint_{};

		//UVの位置
		MLEngine::Math::Vector2 uvTranslate_{};

		//UV回転
		float uvRotate_ = 0.0f;

		//UVスケール
		MLEngine::Math::Vector2 uvScale_{ 1.0f,1.0f };

		//色
		MLEngine::Math::Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

		//インスタンシングの数
		uint32_t maxInstanceCount_;

		//表示するインスタンスの数
		uint32_t instanceCount_ = 0;

	private:

		//静的メンバ変数

		//デバイス
		static ID3D12Device* device_;
		//コマンドリスト
		static ID3D12GraphicsCommandList* commandList_;
		//ルートシグネチャ
		static ID3D12RootSignature* rootSignature_;
		//PSO
		static ID3D12PipelineState* particlePipelineStates_;

		/*static ID3D12PipelineState* pipelineState_;*/

	private:

		//頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
		Microsoft::WRL::ComPtr<ID3D12Resource> matBuff_;
		//インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
		//定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;

		//頂点バッファマップ
		VertexData* vertMap_ = nullptr;
		//インデックスバッファマップ
		uint32_t* indexMap_ = nullptr;
		//定数バッファマップ
		Material2D* constMap_ = nullptr;

		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView_{};
		//インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView_{};

		//TransformMatrix
		Particle2DForGPU* matTransformMap_ = nullptr;

		//テクスチャ
		MLEngine::Resource::Texture texture_;

		std::string texturePath_;

		//インスタンシングリソース
		MLEngine::Resource::InstancingResource instancingResource_;

	};

}


