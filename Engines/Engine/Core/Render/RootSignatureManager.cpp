#include "RootSignatureManager.h"
#include <cassert>

using namespace MLEngine::Core::Render::RootSignature;

Manager* Manager::GetInstance() {
	static Manager instance;
	return &instance;
}

void Manager::Initialize(ID3D12Device* device) {

	assert(device);

	device_ = device;

}

void Manager::CreateRootSignature(Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob, const std::string& name) {

	//既に作成済みなら早期リターン
	if (rootSignatures_.find(name) != rootSignatures_.end()) {

		return;

	}

	HRESULT hr;

	//バイナリを元に生成
	hr = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignatures_[name]));
	assert(SUCCEEDED(hr));


}

ID3D12RootSignature* Manager::GetRootSignature(const std::string& name) {
	//作成済みのものが存在していたらそれを返す
	if (rootSignatures_.find(name) != rootSignatures_.end()) {

		return rootSignatures_[name].Get();

	}

	return nullptr;

}
