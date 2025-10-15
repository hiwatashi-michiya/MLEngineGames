#include "InputLayout.h"

using namespace MLEngine::Core::Render::Config;

InputLayout::InputLayout()
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::SetElements(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElement)
{

	inputLayout_.pInputElementDescs = inputElement.data();
	inputLayout_.NumElements = UINT(inputElement.size());

}
