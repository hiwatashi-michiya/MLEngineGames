#include "InstancingResource.h"
#include "TextureManager.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Core;

void InstancingResource::Initialize(uint32_t instanceCount, Microsoft::WRL::ComPtr<ID3D12Resource> mapResource)
{

	instancingResource_ = TextureManager::GetInstance()->SetInstancingResource(instanceCount, mapResource);

}
