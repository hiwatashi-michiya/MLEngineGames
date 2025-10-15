#pragma once
#include "Render/ResourceView.h"
#include <stdint.h>

namespace MLEngine::Resource {

	/// <summary>
	/// インスタンシングリソースクラス
	/// </summary>
	class InstancingResource
	{
	public:
		InstancingResource() = default;
		~InstancingResource() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="instanceCount"></param>
		/// <param name="mapResource"></param>
		void Initialize(uint32_t instanceCount, Microsoft::WRL::ComPtr<ID3D12Resource> mapResource);

		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() { return instancingResource_.srvHandleGPU; }

	private:

		//インスタンシングリソースのハンドルデータ
		MLEngine::Core::Render::ResourceView instancingResource_;

	};

}
