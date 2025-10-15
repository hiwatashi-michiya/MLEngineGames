#pragma once
#include "Render/ResourceView.h"
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

		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() { return texture_->srvHandleGPU; }

		ID3D12Resource* GetResource() { return texture_->resource.Get(); }

	private:

		//テクスチャのハンドルデータ
		MLEngine::Core::Render::ResourceView* texture_;

	};

	

}
