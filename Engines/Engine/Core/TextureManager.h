#pragma once
#include <array>
#include <d3d12.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <wrl.h>
#include "Render/ResourceView.h"

namespace MLEngine::Core {

	/// <summary>
	/// テクスチャ管理クラス
	/// </summary>
	class TextureManager
	{
	public:
		//インスタンス取得
		static TextureManager* GetInstance();
		//初期化
		void Initialize(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap);
		//テクスチャ読み込み
		Render::ResourceView* Load(const std::string& filePath);
		//インスタンシングリソースセット
		Render::ResourceView SetInstancingResource(uint32_t instanceCount, Microsoft::WRL::ComPtr<ID3D12Resource> mapResource);
		//終了処理
		void Finalize();
		//SRVデスクリプタヒープ取得
		ID3D12DescriptorHeap* GetSRVDescHeap() { return srvDescHeap_.Get(); }
		//テクスチャの番号取得
		const uint32_t& GetTextureIndex() { return textureIndex_; }
		//インデックス加算
		void AddIndex() { textureIndex_++; }

	private:
		//シングルトン化
		TextureManager() = default;
		~TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		const TextureManager& operator=(const TextureManager&) = delete;

	private:

		uint32_t descriptorSizeSRV_;

		ID3D12Device* device_;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescHeap_;
		std::unordered_map<std::string, std::unique_ptr<Render::ResourceView>> textureMap_;

		//intermediateResourceを保持しておくためのvector
		std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID3D12Resource>> intermediateResources_;

		//ロードしたテクスチャの数
		uint32_t textureIndex_ = 0;

	};

}



