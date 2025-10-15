#include "DescriptorRange.h"

using namespace MLEngine::Core::Render::Config;

DescriptorRange::DescriptorRange()
{
}

DescriptorRange::~DescriptorRange()
{
}

void DescriptorRange::SetDescriptorRange(uint32_t baseShaderRegister, uint32_t numDescriptors, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, uint32_t offset, uint32_t descriptorRangeIndex)
{
	//サイズより大きい値なら早期リターン
	if (descriptorRangeIndex >= descriptorRanges_.size()) {
		return;
	}

	descriptorRanges_[descriptorRangeIndex].BaseShaderRegister = baseShaderRegister;
	descriptorRanges_[descriptorRangeIndex].NumDescriptors = numDescriptors;
	descriptorRanges_[descriptorRangeIndex].RangeType = rangeType;
	descriptorRanges_[descriptorRangeIndex].OffsetInDescriptorsFromTableStart = offset;

}
