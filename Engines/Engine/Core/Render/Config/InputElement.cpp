#include"InputElement.h"

using namespace MLEngine::Core::Render::Config;

InputElement::InputElement()
{
}

InputElement::~InputElement()
{
}

void InputElement::SetElement(const char* semanticName, uint32_t semanticIndex, DXGI_FORMAT format, uint32_t alignedByteIndex, uint32_t elementIndex)
{
	//サイズより大きい値なら早期リターン
	if (elementIndex >= inputElement_.size()) {
		return;
	}

	inputElement_[elementIndex].SemanticName = semanticName;
	inputElement_[elementIndex].SemanticIndex = semanticIndex;
	inputElement_[elementIndex].Format = format;
	inputElement_[elementIndex].AlignedByteOffset = alignedByteIndex;

}
