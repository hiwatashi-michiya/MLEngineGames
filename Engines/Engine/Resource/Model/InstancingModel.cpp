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

	assert(DirectXSetter::GetInstance()->GetDevice());

	if (Render::Mesh::Manager::GetInstance()->IsExistMesh(filename)) {

		mesh_ = Render::Mesh::Manager::GetInstance()->GetMesh(filename);

	}
	else {

		//メッシュを登録
		Render::Mesh::Manager::GetInstance()->CreateMesh(filename);
		mesh_ = Render::Mesh::Manager::GetInstance()->GetMesh(filename);

	}

	material_ = std::make_unique<Graphics::Material>();
	material_->Create();

	texture_.Load(mesh_->textureFilePath_);

	//transformMatrix
	{

		matBuff_ = CreateBufferResource(DirectXSetter::GetInstance()->GetDevice(), sizeof(InstancingForGPU) * kMaxInstance_);

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

	//カメラ設定
	{

		cameraBuff_ = CreateBufferResource(DirectXSetter::GetInstance()->GetDevice(), sizeof(CameraForGPU));

		cameraBuff_->SetName(L"cameraBuff");

		cameraBuff_->Map(0, nullptr, reinterpret_cast<void**>(&cameraMap_));

		cameraMap_->worldPosition = Vector3::Zero();

		cameraBuff_->Unmap(0, nullptr);

	}

	//インスタンシングリソース設定
	{

		instancingResource_.Initialize(kMaxInstance_, matBuff_);

	}

}


void InstancingModel::Render(ID3D12GraphicsCommandList* commandList)
{

	//カメラ設定
	commandList->SetGraphicsRootConstantBufferView(4, cameraBuff_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, matBuff_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, texture_.GetGPUHandle());
	commandList->SetGraphicsRootDescriptorTable(6, instancingResource_.GetGPUHandle());

	//描画
	material_->SetCommandMaterial(commandList);

	mesh_->SetCommandMesh(commandList, instanceCount_);
	//インスタンスカウントリセット
	instanceCount_ = 0;

}

void InstancingModel::SetTexture(const std::string& name) {

	texture_ .Load(name);

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

void InstancingModel::AddModelData(RigidModel* model)
{
	//最大数を超えていたら追加しない
	if (instanceCount_ >= kMaxInstance_) {
		return;
	}

	matTransformMap_[instanceCount_].WVP = model->worldViewProjectionMatrix_;
	matTransformMap_[instanceCount_].World = model->localMatrix_ * model->worldMatrix_;
	matTransformMap_[instanceCount_].WorldInverseTranspose = Transpose(Inverse(model->localMatrix_ * model->worldMatrix_));
	matTransformMap_[instanceCount_].color = model->color_;

	AddInstanceCount();

}

void InstancingModel::SetCamera(Camera* camera)
{

	cameraMap_->worldPosition = camera->GetWorldPosition();

}
