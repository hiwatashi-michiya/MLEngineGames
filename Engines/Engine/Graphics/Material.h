#pragma once
#include "RenderingData.h"
#include <d3d12.h>
#include <wrl.h>
#include "Texture/Texture.h"

namespace MLEngine::Graphics {

	/// <summary>
	/// マテリアルデータを保持するクラス
	/// </summary>
	class Material
	{
	public:
		//静的初期化
		static void StaticInitialize(ID3D12Device* device);
		//生成
		Material* Create();

		//マテリアルファイル読み込み
		void LoadMaterialTemplateFile(const std::string& filename);

		//描画コマンドセット
		void SetCommandMaterial(ID3D12GraphicsCommandList* commandList);

		//パーティクル用
		void SetCommandMaterialForParticle(ID3D12GraphicsCommandList* commandList);

		//テクスチャセット
		void SetTexture(MLEngine::Resource::Texture texture) { texture_ = texture; }

		//定数バッファ取得関数
		Microsoft::WRL::ComPtr<ID3D12Resource> GetConstBuffer() { return constBuff_; }

		//ImGui表示
		void ImGuiUpdate();

		//定数バッファマップ
		MLEngine::Resource::MaterialData* constMap = nullptr;
		//ポイントライト
		MLEngine::Resource::PointLight* pLightMap = nullptr;

		//テクスチャ
		MLEngine::Resource::Texture texture_;

	private:

		static ID3D12Device* device_;

		//定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
		//点光源バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> pLightBuff_;

	};

}



