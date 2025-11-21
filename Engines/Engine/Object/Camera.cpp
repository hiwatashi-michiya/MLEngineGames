#include "Camera.h"
#include "DirectXSetter.h"
#include "BufferResource.h"

using namespace MLEngine::Math;
using namespace MLEngine::Object;
using namespace MLEngine::Core;

Camera::Camera()
{

	ID3D12Device* device = DirectXSetter::GetInstance()->GetDevice();

	//平行光源バッファ設定
	{

		buffer_ = CreateBufferResource(device, sizeof(CBData));

		buffer_->SetName(L"dLightBuff");

		buffer_->Map(0, nullptr, reinterpret_cast<void**>(&cbData));

		cbData->worldPosition = Vector3::Zero();

		buffer_->Unmap(0, nullptr);

	}

}

void Camera::Initialize() {
	//行列の初期化
	matProjection_ = MakePerspectiveFovMatrix(0.45f, float(1280.0f) / float(720.0f), 0.1f, 500.0f);

	matWorld_ = MakeScaleMatrix(scale_) * matRotate_ * MakeTranslateMatrix(position_);

	matView_ = Inverse(matWorld_);

}

void Camera::Update() {
	//回転更新
	rotateQuaternion_ = ConvertFromEuler(rotation_);
	matRotate_ = MakeRotateMatrix(rotation_);
	//行列更新
	matWorld_ = MakeScaleMatrix(scale_) * matRotate_ * MakeTranslateMatrix(position_);
	matView_ = Inverse(matWorld_);
	matViewProjection_ = matView_ * matProjection_;
	cbData->worldPosition = GetWorldPosition();
}

void MLEngine::Object::Camera::SetCameraCBV(UINT index)
{
	ID3D12GraphicsCommandList* commandList = DirectXSetter::GetInstance()->GetCommandList();
	commandList->SetGraphicsRootConstantBufferView(index, buffer_->GetGPUVirtualAddress());
}
