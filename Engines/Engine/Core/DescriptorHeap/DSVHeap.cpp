#include "DSVHeap.h"

using namespace MLEngine::Core;

DSVHeap::DSVHeap() : IDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)
{
	Create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kMaxDescriptor_, false);
}

DSVHeap::~DSVHeap()
{
}
