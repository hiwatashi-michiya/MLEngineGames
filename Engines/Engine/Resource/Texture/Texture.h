#pragma once
#include "Render/ResourceView.h"
#include "TextureManager.h"
#include <string>

namespace MLEngine::Resource {

	/// <summary>
	/// テクスチャクラス
	/// </summary>
	class Texture
	{
	public:
		Texture() = default;
		~Texture() = default;

		/// <summary>
		/// ファイル読み込み
		/// </summary>
		/// <param name="filename"></param>
		void Load(const std::string& filename);

		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return MLEngine::Core::TextureManager::GetInstance()->GetTexture(index_)->srvHandleGPU; }

		ID3D12Resource* GetResource() { return MLEngine::Core::TextureManager::GetInstance()->GetTexture(index_)->resource.Get(); }

		uint32_t GetIndex() const { return index_; }

	private:

		//テクスチャのハンドルデータ
		uint32_t index_;

	};

	

}
