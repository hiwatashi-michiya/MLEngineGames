#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_6.h>

namespace MLEngine::Core {

	class DXDevice
	{
	public:

		//インスタンス取得
		static DXDevice* GetInstance();

		void Initialize();

		IDXGIFactory7* GetFactory() { return dxgiFactory_.Get(); }

		ID3D12Device* GetDevice() { return device_.Get(); }

	private:

		//ファクトリー
		Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
		//デバイス
		Microsoft::WRL::ComPtr<ID3D12Device> device_;

	private:
		//シングルトン化
		DXDevice() = default;
		~DXDevice() = default;
		DXDevice(const DXDevice&) = delete;
		const DXDevice& operator=(const DXDevice&) = delete;


	};

}
