#include "Texture.h"
#include "TextureManager.h"
#include "PreLoader.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Core;

void Texture::Load(const std::string& filename) {

	//リソースビューからインデックスに変更する
	index_ = TextureManager::GetInstance()->Load(filename);

	PreLoader::GetInstance()->AddTexture(*this);

}
