#include "DescriptorHeap.h"
#include <cassert>
#include <Windows.h>

using namespace MLEngine::Core;

DescriptorHeap::DescriptorHeap() {

}

DescriptorHeap::~DescriptorHeap() {

}

void DescriptorHeap::Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors, bool shaderVisible, const std::string& name) {

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

	//セットネームを後でやる
	name;

	limit_ = numDescriptors;

	isUsed_.resize(limit_);

}

const uint32_t& DescriptorHeap::GetCurrentIndex() {

	assert(index_ < limit_);

	return index_;

}

uint32_t DescriptorHeap::AddIndex() {

	assert(index_ < limit_);

	return index_++;

}

uint32_t DescriptorHeap::GetUnUsedIndex()
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

void DescriptorHeap::SetIndexUnused(uint32_t index)
{

	assert(index < limit_);

	if (isUsed_[index]) {
		isUsed_[index] = false;
	}

}
