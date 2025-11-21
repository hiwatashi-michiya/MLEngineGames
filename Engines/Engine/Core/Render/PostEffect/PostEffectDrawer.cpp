#include "PostEffectDrawer.h"
#include <cassert>
#include "Core/DescriptorHandle.h"
#include "ImGuiManager.h"

using namespace MLEngine::Core::Render::PostEffect;
using namespace MLEngine::Object;
using namespace MLEngine::Math;

PostEffectDrawer* PostEffectDrawer::GetInstance() {
	static PostEffectDrawer instance;
	return &instance;
}

void PostEffectDrawer::Initialize() {

	dxSetter_ = DirectXSetter::GetInstance();

	device_ = dxSetter_->GetDevice();

	//DescriptorSizeを取得しておく
	uint32_t descriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	uint32_t descriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (int32_t i = 0; i < 2; i++) {

		RenderTexture newRenderTex;

		uint32_t handleIndex = dxSetter_->GetSrvHeap()->GetUnUsedIndex();

		//レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		//RenderTexture用のRTVを作成
		const Vector4 kRenderTargetClearValue{ 0.05f,0.1f,0.5f,1.0f}; //青

		//ディスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetCPUDescriptorHandle(dxSetter_->GetRtvHeap()->Get(), descriptorSizeRTV, 2 + i);
		newRenderTex.Create(device_, Window::Manager::GetInstance()->GetClientWidth(), Window::Manager::GetInstance()->GetClientHeight(),
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
		device_->CreateRenderTargetView(newRenderTex.Get(), &renderTargetViewDesc, rtvHandle);

		//SRVの設定。FormatはResourceと同じ
		D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
		renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		renderTextureSrvDesc.Texture2D.MipLevels = 1;

		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU =
			GetCPUDescriptorHandle(dxSetter_->GetSrvHeap()->Get(), descriptorSizeSRV, handleIndex);

		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU =
			GetGPUDescriptorHandle(dxSetter_->GetSrvHeap()->Get(), descriptorSizeSRV, handleIndex);

		newRenderTex.SetCPUHandle(srvHandleCPU);
		newRenderTex.SetGPUHandle(srvHandleGPU);

		device_->CreateShaderResourceView(newRenderTex.Get(), &renderTextureSrvDesc, srvHandleCPU);

		renderTextures_.push_back(newRenderTex);

	}
	//ここから各ポストエフェクト追加
	postEffects_.push_back(std::make_shared<CopyImageRender>());

	postEffects_.push_back(std::make_shared<Grayscale>());

	postEffects_.push_back(std::make_shared<Vignette>());

	postEffects_.push_back(std::make_shared<BoxFilter>());

	postEffects_.push_back(std::make_shared<GaussianFilter>());

	postEffects_.push_back(std::make_shared<LuminanceBasedOutline>());

	postEffects_.push_back(std::make_shared<DepthBasedOutline>());

	postEffects_.push_back(std::make_shared<RadialBlur>());

	postEffects_.push_back(std::make_shared<HSVFilter>());

	postEffects_.push_back(std::make_shared<Dissolve>());

	postEffects_.push_back(std::make_shared<Paper>());

	for (int32_t i = 0; i < postEffects_.size(); i++) {

		postEffects_[i]->Create();

	}

}

void PostEffectDrawer::Draw(int32_t drawNum) {

	//最初の一回だけ全部描画する
	/*if (not isUsedAllEffects_) {

		for (int32_t i = 0; i < postEffects_.size(); i++) {
			postEffects_[i]->Render();
			postEffects_[i]->PostRender();
		}

		isUsedAllEffects_ = true;

	}*/
	//指定したタイプのエフェクト描画
	if (type_ < postEffects_.size()) {
		postEffects_[type_]->Render();
	}

	//書き込み先を設定
	renderTextures_[drawNum].Draw(renderTextures_[drawNum].GetGPUHandle());

	if (type_ < postEffects_.size()) {
		postEffects_[type_]->PostRender();
	}

}

void PostEffectDrawer::SetBarrier(int32_t index, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{

	Core::DirectXFunction::SetBarrier(D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_BARRIER_FLAG_NONE,
		before, after, renderTextures_[index].Get());

}

void PostEffectDrawer::Debug() {

#ifdef _DEBUG

	//各ポストエフェクトのデバッグ処理
	ImGui::Begin("PostEffects", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("Normal")) {
		type_ = kNone;
	}

	if (ImGui::Button("GrayScale")) {
		type_ = kGrayscale;
	}

	if (ImGui::Button("Vignette")) {
		type_ = kVignette;
	}

	if (ImGui::Button("BoxFilter")) {
		type_ = kBoxFilter;
	}

	if (ImGui::Button("GaussianFilter")) {
		type_ = kGaussianFilter;
	}

	if (ImGui::BeginTabBar("PostEffectsItem")) {

		for (int32_t i = 0; i < postEffects_.size(); i++) {

			if (ImGui::BeginTabItem(postEffects_[i]->name_.c_str())) {

				postEffects_[i]->Debug();

				ImGui::EndTabItem();

			}

		}

		ImGui::EndTabBar();

	}

	ImGui::End();

#endif // _DEBUG

}
