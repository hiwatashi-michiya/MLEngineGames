#include "PreLoader.h"

using namespace MLEngine::Resource;

PreLoader* PreLoader::GetInstance() {

	static PreLoader instance;
	return &instance;

}

void PreLoader::DrawAllTextures()
{

	if (not textures_.empty()) {
		
	}

}
