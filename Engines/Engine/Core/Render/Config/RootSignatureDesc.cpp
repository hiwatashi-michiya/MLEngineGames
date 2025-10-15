#include "RootSignatureDesc.h"

using namespace MLEngine::Core::Render::Config;

RootSignatureDesc::RootSignatureDesc()
{
}

RootSignatureDesc::~RootSignatureDesc()
{
}

void RootSignatureDesc::SetFlags(D3D12_ROOT_SIGNATURE_FLAGS flag)
{

	rootSignatureDesc_.Flags = flag;

}

void RootSignatureDesc::SetRootParameter(const std::vector<D3D12_ROOT_PARAMETER>& rootParameters)
{

	rootSignatureDesc_.pParameters = rootParameters.data();
	rootSignatureDesc_.NumParameters = UINT(rootParameters.size());

}

void RootSignatureDesc::SetSamplers(const std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers)
{

	rootSignatureDesc_.pStaticSamplers = samplers.data();
	rootSignatureDesc_.NumStaticSamplers = UINT(samplers.size());

}
