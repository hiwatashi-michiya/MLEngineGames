#include "DirectionalLight.h"
#include "BufferResource.h"
#include "DXDevice.h"
#include "DirectXSetter.h"
#include "ImguiManager.h"

using namespace MLEngine::Lighting;
using namespace MLEngine::Math;
using namespace MLEngine::Core;

DirectionalLight::DirectionalLight()
{
	
	ID3D12Device* device = DXDevice::GetInstance()->GetDevice();

	//平行光源バッファ設定
	{

		buffer_ = CreateBufferResource(device, sizeof(CBData));

		buffer_->SetName(L"dLightBuff");

		buffer_->Map(0, nullptr, reinterpret_cast<void**>(&cbData));

		cbData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		cbData->normalDirection = { 0.0f,-1.0f,0.0f };
		cbData->intensity = 1.0f;

		buffer_->Unmap(0, nullptr);

	}

}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::SetLightCBV(UINT index)
{

	//無理やり入れているので後ほど分離
	cbData->normalDirection = Normalize(direction);

	ID3D12GraphicsCommandList* commandList = DirectXSetter::GetInstance()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(index, buffer_->GetGPUVirtualAddress());
}

void DirectionalLight::Debug() {

#ifdef _DEBUG

	ImGui::DragFloat("intensity", &cbData->intensity);
	ImGui::DragFloat3("direction", &direction.x);

#endif // _DEBUG


}
