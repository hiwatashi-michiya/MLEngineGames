#pragma once
#include <d3d12.h>
#include <vector>

namespace MLEngine::Core::Render::Config {

	/// <summary>
	/// ルートシグネチャデスクを扱いやすくしたクラス
	/// </summary>
	class RootSignatureDesc
	{
	public:
		RootSignatureDesc();
		~RootSignatureDesc();
		//フラグセット
		void SetFlags(D3D12_ROOT_SIGNATURE_FLAGS flag);
		//ルートパラメータセット
		void SetRootParameter(const std::vector<D3D12_ROOT_PARAMETER>& rootParameters);
		//サンプラーセット
		void SetSamplers(const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers);
		//ルートシグネチャデスクの本体取得
		const D3D12_ROOT_SIGNATURE_DESC& Get() const { return rootSignatureDesc_; }

	private:

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc_{};

	};

}




