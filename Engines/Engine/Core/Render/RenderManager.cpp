#include "RenderManager.h"
#include "Core/DirectXSetter.h"
#include "PostEffect/PostEffectDrawer.h"
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

void Manager::Clear()
{

	models_.clear();
	skinningModels_.clear();
	particles_.clear();
	sprites_.clear();
	lines_.clear();

}

void Manager::Render()
{
	//レンダーテクスチャの対象設定
	MLEngine::Core::DirectXSetter::GetInstance()->RenderTexturePreDraw(1);
	//モデル描画
	Model::Manager::GetInstance()->PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());

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
	//ここまでがポストエフェクトの対象
	MLEngine::Core::DirectXSetter::GetInstance()->PreDraw();
	//ポストエフェクト適用
	MLEngine::Core::Render::PostEffect::PostEffectDrawer::GetInstance()->Draw(1);

	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(0);
	////ポストエフェクト適用
	//PostEffectDrawer::GetInstance()->SetType(PostEffectType::kGrayscale);
	//PostEffectDrawer::GetInstance()->Draw(0);
	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(1);
	////ポストエフェクト適用
	//PostEffectDrawer::GetInstance()->SetType(PostEffectType::kVignette);
	//PostEffectDrawer::GetInstance()->Draw(1);
	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(0);
	////ポストエフェクト適用
	//PostEffectDrawer::GetInstance()->SetType(PostEffectType::kRadialBlur);
	//PostEffectDrawer::GetInstance()->Draw(0);
	////レンダーテクスチャの対象設定
	//DirectXSetter::GetInstance()->RenderTexturePreDraw(1);
	////ポストエフェクト適用
	//PostEffectDrawer::GetInstance()->SetType(PostEffectType::kHSVFilter);
	//PostEffectDrawer::GetInstance()->Draw(1);

	//スプライト描画
	Sprite::PreDraw(MLEngine::Core::DirectXSetter::GetInstance()->GetCommandList());

	for (int32_t i = 0; i < sprites_.size(); i++) {
		sprites_[i]->Render();
	}

	Sprite::PostDraw();
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
