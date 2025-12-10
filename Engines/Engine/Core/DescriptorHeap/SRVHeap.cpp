#include "SRVHeap.h"
#include <cassert>

using namespace MLEngine::Core;

SRVHeap::SRVHeap() : IDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 4096),
kMaxTexture_(512)
{
	Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxDescriptor_, true);
}

SRVHeap::~SRVHeap()
{
}

uint32_t SRVHeap::GetUnUsedIndex()
{
	//テクスチャの最大値の次のインデックスから参照
	for (uint32_t i = kMaxTexture_; i < limit_; i++) {

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

uint32_t SRVHeap::GetUnUsedTextureIndex()
{
	
	for (uint32_t i = 0; i < kMaxTexture_; i++) {

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
