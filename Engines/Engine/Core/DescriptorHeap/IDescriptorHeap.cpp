#include "IDescriptorHeap.h"
#include "DXDevice.h"
#include <cassert>
#include <Windows.h>

using namespace MLEngine::Core;

IDescriptorHeap::IDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t maxDescriptor) : 
	kDescriptorSize_(DXDevice::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(type)),
	kMaxDescriptor_(maxDescriptor)
{

}

void IDescriptorHeap::Create(D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors, bool shaderVisible) {

	ID3D12Device* device = DXDevice::GetInstance()->GetDevice();

	//ディスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
	descriptorHeapDesc.Type = heapType;
	descriptorHeapDesc.NumDescriptors = numDescriptors;
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&heap_));
	//ディスクリプタヒープが作れなかったので起動できない
	if (!SUCCEEDED(hr)) {

		MessageBox(nullptr, L"Failed Create DescriptorHeap", L"DescriptorHeap::Create", 0);

		assert(false);

	}

	limit_ = numDescriptors;

	isUsed_.resize(limit_);

}

//CPUのDescriptorHandle取得
D3D12_CPU_DESCRIPTOR_HANDLE IDescriptorHeap::GetCPUDescriptorHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = heap_->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (kDescriptorSize_ * index);
	return handleCPU;

}

//GPUのDescriptorHandle取得
D3D12_GPU_DESCRIPTOR_HANDLE IDescriptorHeap::GetGPUDescriptorHandle(uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = heap_->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (kDescriptorSize_ * index);
	return handleGPU;

}

uint32_t IDescriptorHeap::GetUnUsedIndex()
{

	for (uint32_t i = 0; i < limit_; i++) {

		//使用されていない場合、そのインデックスを返す
		if (not isUsed_[i]) {
			isUsed_[i] = true;
			return i;
		}

	}

	//使用制限を超えているのでassert
	assert(false);

	return 0;
}

void IDescriptorHeap::SetIndexUnUsed(uint32_t index)
{

	assert(index < limit_);

	if (isUsed_[index]) {
		isUsed_[index] = false;
	}

}
