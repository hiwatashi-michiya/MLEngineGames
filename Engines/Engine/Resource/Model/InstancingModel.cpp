#include "InstancingModel.h"
#include <cassert>
#include "Engine/Convert.h"
#include "Engine/Core/Render/ShaderManager.h"
#include <fstream>
#include <sstream>
#include "Core/ImGuiManager.h"
#include "Core/Render/RootSignatureManager.h"
#include "Buffer/BufferResource.h"
#include "Core/Render/PipelineManager.h"
#include "Core/Render/MeshManager.h"
#include "Core/Render/Config/RootSignatureDesc.h"
#include "Core/Render/Config/RootParameter.h"
#include "Core/Render/Config/StaticSampler.h"
#include "Core/Render/Config/InputElement.h"
#include "Core/Render/Config/InputLayout.h"
#include "Core/Render/Config/DescriptorRange.h"
#include "Core/Render/RenderManager.h"
#include "DirectXSetter.h"
#include "DXDevice.h"

#pragma comment(lib, "dxcompiler.lib")

using namespace MLEngine::Resource;
using namespace MLEngine::Core;
using namespace MLEngine::Math;
using namespace MLEngine::Object;

InstancingModel* InstancingModel::Create(const std::string& filename) {

	InstancingModel* model = new InstancingModel();

	model->Initialize(filename);

	return model;

}

void InstancingModel::Initialize(const std::string& filename) {

	assert(DXDevice::GetInstance()->GetDevice());

	if (Render::Mesh::Manager::GetInstance()->IsExistMesh(filename)) {

		mesh = Render::Mesh::Manager::GetInstance()->GetMesh(filename);

	}
	else {

		//メッシュを登録
		Render::Mesh::Manager::GetInstance()->CreateMesh(filename);
		mesh = Render::Mesh::Manager::GetInstance()->GetMesh(filename);

	}

	material_ = std::make_unique<Graphics::Material>();
	material_->Create(kMaxInstance_);

	//transformMatrix
	{

		matBuff_ = CreateBufferResource(DXDevice::GetInstance()->GetDevice(), sizeof(InstancingForGPU) * kMaxInstance_);

		matBuff_->SetName(L"matrixBuff");

		matBuff_->Map(0, nullptr, reinterpret_cast<void**>(&matTransformMap_));

		//インスタンシングの数だけループ
		for (uint32_t i = 0; i < kMaxInstance_; i++) {
			matTransformMap_[i].WVP = MakeIdentity4x4();
			matTransformMap_[i].World = MakeIdentity4x4();
			matTransformMap_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		}

		matBuff_->Unmap(0, nullptr);

	}

	//インスタンシングリソース設定
	{

		instancingResource_.Initialize(kMaxInstance_, matBuff_, sizeof(InstancingForGPU));

	}

}


void InstancingModel::Render(ID3D12GraphicsCommandList* commandList)
{

	//インスタンスカウントが0以下なら描画の必要はない
	if (instanceCount_ <= 0) {
		return;
	}

	commandList->SetGraphicsRootDescriptorTable(1, instancingResource_.GetGPUHandle());

	commandList->SetGraphicsRootDescriptorTable(2, DirectXSetter::GetInstance()->GetSrvHeap()->GetGPUHandleStart());
	commandList->SetGraphicsRootDescriptorTable(6, instancingResource_.GetGPUHandle());

	//コマンドセット
	material_->SetCommandMaterial(commandList);

	mesh->SetCommandMesh(commandList, instanceCount_);
	//インスタンスカウントリセット
	instanceCount_ = 0;

}

void InstancingModel::ImGuiUpdate(const std::string& name) {

#ifdef _DEBUG

	ImGui::Begin(name.c_str());

	if (ImGui::BeginTabBar("InstancingModel", ImGuiTabBarFlags_None)) {

		if (ImGui::BeginTabItem("material")) {
			material_->ImGuiUpdate();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

#endif // _DEBUG

}

void InstancingModel::Regist(RigidModel* model)
{
	//最大数を超えていたら追加しない
	if (instanceCount_ >= kMaxInstance_) {
		return;
	}

	matTransformMap_[instanceCount_].WVP = model->worldViewProjectionMatrix;
	matTransformMap_[instanceCount_].World = model->localMatrix * model->worldMatrix;
	matTransformMap_[instanceCount_].WorldInverseTranspose = Transpose(Inverse(model->localMatrix * model->worldMatrix));
	matTransformMap_[instanceCount_].color = model->color;
	matTransformMap_[instanceCount_].textureIndex = model->GetTextureIndex();

	material_->SetMaterialData(instanceCount_, model->materialData);

	AddInstanceCount();

}

void InstancingModel::Regist(Sprite3D* sprite3D)
{

	//最大数を超えていたら追加しない
	if (instanceCount_ >= kMaxInstance_) {
		return;
	}

	matTransformMap_[instanceCount_].WVP = sprite3D->worldViewProjectionMatrix;
	matTransformMap_[instanceCount_].World = sprite3D->localMatrix * sprite3D->worldMatrix;
	matTransformMap_[instanceCount_].WorldInverseTranspose = Transpose(Inverse(sprite3D->localMatrix * sprite3D->worldMatrix));
	matTransformMap_[instanceCount_].color = sprite3D->color;

	material_->SetMaterialData(instanceCount_, sprite3D->materialData);

	AddInstanceCount();

}

