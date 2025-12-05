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
		uint32_t Load(const std::string& filePath);
		//インスタンシングリソースセット
		Render::ResourceView SetInstancingResource(uint32_t instanceCount, Microsoft::WRL::ComPtr<ID3D12Resource> mapResource, 
			UINT size);
		//ハンドルからテクスチャを取得
		Render::ResourceView* GetTexture(uint32_t index) { return textureList_[index].get(); }
		//テクスチャの0番目を取得
		Render::ResourceView* GetTexturesFirst();
		//終了処理
		void Finalize();
		//SRVデスクリプタヒープ取得
		ID3D12DescriptorHeap* GetSRVDescHeap() { return srvDescHeap_.Get(); }

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
		//ファイルパスとリソースビューを繋げたマップ
		std::vector<std::unique_ptr<Render::ResourceView>> textureList_;
		//ファイルパスとインデックスを繋げたマップ
		std::unordered_map<std::string, uint32_t> intMap_;
		//intermediateResourceを保持しておくためのvector
		std::unordered_map<uint32_t, Microsoft::WRL::ComPtr<ID3D12Resource>> intermediateResources_;

	};

}



