#include "Sprite3D.h"
#include <cassert>
#include "Engine/Convert.h"
#include "Engine/Core/Render/ShaderManager.h"
#include <fstream>
#include <sstream>
#include "Core/ImGuiManager.h"
#include "Buffer/BufferResource.h"
#include "ModelManager.h"
#include "../Model/InstancingModel.h"
#include "../ResourceManager.h"

#pragma comment(lib, "dxcompiler.lib")

using namespace MLEngine::Resource;
using namespace MLEngine::Core::Render;
using namespace MLEngine::Math;
using namespace MLEngine::Object;

Sprite3D::~Sprite3D()
{
	Resource::Manager::GetInstance()->RemoveSprite3D(this);
}

void Sprite3D::Initialize(const std::string& filename, [[maybe_unused]] const std::string& texturename) {

	localMatrix = Matrix4x4::Identity();
	worldMatrix = Matrix4x4::Identity();
	worldViewProjectionMatrix = Matrix4x4::Identity();
	color = { 1.0f,1.0f,1.0f,1.0f };
	//初期化時に描画用のリストに登録
	Resource::Manager::GetInstance()->AddSprite3D(this);
	//既にインスタンシング用のモデルを作成している場合、それを返す
	if (Model::Manager::GetInstance()->IsExistModel(filename)) {
		instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);
		texture_.Load(instancingModel_->mesh->GetTextureFilePath());
		return;
	}
	//新規でインスタンシング用のモデルを作成
	Model::Manager::GetInstance()->AddModel(filename);
	//ポインタを渡す
	instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);
	texture_.Load(instancingModel_->mesh->GetTextureFilePath());

}

void Sprite3D::UpdateAnimation() {

	

}

void Sprite3D::Regist()
{

	if (instancingModel_) {
		instancingModel_->Regist(this);
	}

}

void Sprite3D::SetMesh(const std::string& filename, [[maybe_unused]] const std::string& texturename)
{

	//既にインスタンシング用のモデルを作成している場合、それを返す
	if (Model::Manager::GetInstance()->IsExistModel(filename)) {
		instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);
		return;
	}
	//新規でインスタンシング用のモデルを作成
	Model::Manager::GetInstance()->AddModel(filename);
	//ポインタを渡す
	instancingModel_ = Model::Manager::GetInstance()->GetModel(filename);

}

