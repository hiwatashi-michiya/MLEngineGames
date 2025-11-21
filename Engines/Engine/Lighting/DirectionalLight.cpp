#include "DirectionalLight.h"
#include "BufferResource.h"
#include "DirectXSetter.h"

using namespace MLEngine::Lighting;
using namespace MLEngine::Math;
using namespace MLEngine::Core;

DirectionalLight::DirectionalLight()
{
	
	ID3D12Device* device = DirectXSetter::GetInstance()->GetDevice();

	//平行光源バッファ設定
	{

		buffer_ = CreateBufferResource(device, sizeof(CBData));

		buffer_->SetName(L"dLightBuff");

		buffer_->Map(0, nullptr, reinterpret_cast<void**>(&cbData));

		cbData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		cbData->direction = { 0.0f,1.0f,-1.0f };
		cbData->intensity = 0.0f;

		buffer_->Unmap(0, nullptr);

	}

}

DirectionalLight::~DirectionalLight()
{
}

void MLEngine::Lighting::DirectionalLight::SetLightCBV(UINT index)
{
	ID3D12GraphicsCommandList* commandList = DirectXSetter::GetInstance()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(index, buffer_->GetGPUVirtualAddress());
}
