#pragma once
#include <d3d12.h>
#include <wrl.h>

namespace MLEngine::Core::Render {

	//リソースを読み取るための構造体
	struct ResourceView {

		//リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		//CPUハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		//GPUハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;

	};

}
