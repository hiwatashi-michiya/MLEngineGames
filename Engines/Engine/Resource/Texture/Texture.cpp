#include "Texture.h"
#include "TextureManager.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Core;

void Texture::Load(const std::string& filename) {

	texture_ = TextureManager::GetInstance()->Load(filename);

}
