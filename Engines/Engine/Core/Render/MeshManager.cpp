#include "MeshManager.h"

using namespace MLEngine::Core::Render::Mesh;

Manager* Manager::GetInstance() {
	static Manager instance;
	return &instance;
}

void Manager::CreateMesh(const std::string& fileName) {

	//メッシュを登録
	meshes_[fileName] = std::make_shared<MLEngine::Graphics::Mesh>();
	meshes_[fileName]->Create(fileName);

}
