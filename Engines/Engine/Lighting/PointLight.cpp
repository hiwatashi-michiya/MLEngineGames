#include "PointLight.h"
#include "BufferResource.h"
#include "DirectXSetter.h"
#include "DXDevice.h"

using namespace MLEngine::Lighting;
using namespace MLEngine::Math;
using namespace MLEngine::Core;

PointLight::PointLight() {

	ID3D12Device* device = DXDevice::GetInstance()->GetDevice();

	//平行光源バッファ設定
	{

		buffer_ = CreateBufferResource(device, sizeof(CBData));

		buffer_->SetName(L"dLightBuff");

		buffer_->Map(0, nullptr, reinterpret_cast<void**>(&cbData));

		cbData->color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
		cbData->position = Vector3{ 0.0f,0.0f,0.0f };
		cbData->intensity = 1.0f;
		cbData->radius = 200.0f;
		cbData->decay = 1.0f;

		buffer_->Unmap(0, nullptr);

	}


}

PointLight::~PointLight() {

}

void PointLight::SetLightCBV(UINT index) {
	ID3D12GraphicsCommandList* commandList = DirectXSetter::GetInstance()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(index, buffer_->GetGPUVirtualAddress());
}
