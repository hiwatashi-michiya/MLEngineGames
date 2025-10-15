#include "StaticSampler.h"

using namespace MLEngine::Core::Render::Config;

StaticSampler::StaticSampler()
{
}

StaticSampler::~StaticSampler()
{
}

void StaticSampler::SetSampler(D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE address, D3D12_COMPARISON_FUNC comparisonFunc, float maxLOD, uint32_t shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility, uint32_t samplerIndex)
{

	//サイズが範囲外なら早期リターン
	if (samplerIndex >= staticSamplers_.size()) {
		return;
	}

	//値を実際のサンプラーに代入
	staticSamplers_[samplerIndex].Filter = filter;
	staticSamplers_[samplerIndex].AddressU = address;
	staticSamplers_[samplerIndex].AddressV = address;
	staticSamplers_[samplerIndex].AddressW = address;
	staticSamplers_[samplerIndex].ComparisonFunc = comparisonFunc;
	staticSamplers_[samplerIndex].MaxLOD = maxLOD;
	staticSamplers_[samplerIndex].ShaderRegister = shaderRegister;
	staticSamplers_[samplerIndex].ShaderVisibility = shaderVisibility;

}
