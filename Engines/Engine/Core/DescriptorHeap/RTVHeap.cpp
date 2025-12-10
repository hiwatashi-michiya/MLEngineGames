#include "RTVHeap.h"

using namespace MLEngine::Core;

RTVHeap::RTVHeap() : IDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 4)
{
	Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxDescriptor_, false);
}

RTVHeap::~RTVHeap()
{
}
