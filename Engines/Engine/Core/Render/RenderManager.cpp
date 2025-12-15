#include "RenderManager.h"
#include "Core/DirectXSetter.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"

using namespace MLEngine::Core::Render;
using namespace MLEngine::Resource;

Manager* Manager::GetInstance()
{
	static Manager instance;
	return &instance;
}

void Manager::Initialize() {

	postEffect_ = PostEffect::PostEffectDrawer::GetInstance();
	Clear();

}

void Manager::Clear()
{

	skinningModels_.clear();
	particles_.clear();
	sprite2Ds_.clear();
	lines_.clear();

}

void Manager::Render()
{
	//レンダーテクスチャの対象設定
	MLEngine::Core::DirectXSetter::GetInstance()->RenderTexturePreDraw(0);

	//モデル描画
	Model::Manager::GetInstance()->PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());
	//ライトのCBuffer設定(3番目に設定)
	dLight_->SetLightCBV(3);
	//カメラのCBuffer設定(4番目に設定)
	camera_->SetCameraCBV(4);
	Model::Manager::GetInstance()->Render();

	Model::Manager::GetInstance()->PostDraw();
	//スキニングモデル描画描画
	SkinningModel::PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());

	for (int32_t i = 0; i < skinningModels_.size(); i++) {
		skinningModels_[i]->Render();
	}

	SkinningModel::PostDraw();
	//パーティクル描画
	Particle3D::PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());

	for (int32_t i = 0; i < particles_.size(); i++) {
		particles_[i]->Render();
	}

	Particle3D::PostDraw();
	//レンダーテクスチャの対象設定
	DirectXSetter::GetInstance()->RenderTexturePreDraw(1);
	//0番目にモデルの描画を全てしているので、0番目をシェーダーリソースとして扱って1番目のリソースに描画する

	//ポストエフェクト適用
	postEffect_->SetType(PostEffect::PostEffectType::kNone);
	postEffect_->SetBarrier(0, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	postEffect_->Draw(0);
	postEffect_->SetBarrier(0, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(0);
	////1番目に書き込みがされたので、それを利用して0番目に上書き

	////ポストエフェクト適用
	//postEffect_->SetType(PostEffect::PostEffectType::kGrayscale);
	//postEffect_->SetBarrier(1, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//postEffect_->Draw(1);
	//postEffect_->SetBarrier(1, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(1);
	////適用したいエフェクトが無くなるまで繰り返し

	////ポストエフェクト適用
	//postEffect_->SetType(PostEffect::PostEffectType::kVignette);
	//postEffect_->SetBarrier(0, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//postEffect_->Draw(0);
	//postEffect_->SetBarrier(0, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(0);
	////ポストエフェクト適用
	//postEffect_->SetType(PostEffect::PostEffectType::kRadialBlur);
	//postEffect_->SetBarrier(1, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//postEffect_->Draw(1);
	//postEffect_->SetBarrier(1, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(1);
	////ポストエフェクト適用
	//postEffect_->SetType(PostEffect::PostEffectType::kHSVFilter);
	//postEffect_->SetBarrier(0, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	//postEffect_->Draw(0);
	//postEffect_->SetBarrier(0, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//ここまでがポストエフェクトの対象
	MLEngine::Core::DirectXSetter::GetInstance()->PreDraw();
	postEffect_->SetBarrier(1, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	postEffect_->Draw(1);
	postEffect_->SetBarrier(1, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//スプライト描画
	Sprite2D::PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());

	for (int32_t i = 0; i < sprite2Ds_.size(); i++) {
		sprite2Ds_[i]->Render();
	}

	Sprite2D::PostDraw();
	//ライン描画
	Line::PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());

	for (int32_t i = 0; i < lines_.size(); i++) {
		lines_[i]->Render();
	}

	Line::PostDraw();
	//描画対象のリセット
	Clear();

#ifdef _DEBUG

	ImGuiManager::GetInstance()->Render();

#endif // _DEBUG

	//描画後処理。バリアを張る
	MLEngine::Core::DirectXSetter::GetInstance()->PostDraw();

}
