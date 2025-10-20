#include "DebugScene.h"
#include "ImguiManager.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Object::Collision;

DebugScene::DebugScene()
{
}

DebugScene::~DebugScene()
{
}

inline void DebugScene::Initialize()
{
	//お試しプッシュ

	input_ = MLEngine::Input::Manager::GetInstance();

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-10.0f };

	model_.Initialize("./Resources/EngineResources/testObjects/axis.obj");
	particle_.reset(Particle3D::Create("./Resources/model/plane/plane.obj", 32));
	//読み込み("./Resources/audio/"以降のパスでOK)
	se1_.Load("SE/test.mp3");

	box_.SetCollisionAttribute(0x00000002);
	//当たった瞬間の呼び出し関数
	box_.SetEnterFunction([this](Collider* collider) {lineBox_.SetColor({ 1.0f,0.0f,0.0f,1.0f }); });
	//当たっている時の呼び出し関数
	box_.SetStayFunction([this](Collider* collider) {lineBox_.SetColor({ 0.0f,1.0f,0.0f,1.0f }); });
	//離れた瞬間の呼び出し関数
	box_.SetExitFunction([this](Collider* collider) {lineBox_.SetColor({ 1.0f,1.0f,1.0f,1.0f }); });

	lineBox_.SetOBB(&box_.collider_);
	sphere_.SetCollisionAttribute(0x00000001);
	lineSphere_.SetSphere(&sphere_.collider_);

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{


	//Audio
	{

#ifdef _DEBUG

		ImGui::Begin("Test");

		ImGui::Text("isPlayingSE: %d", se1_.IsPlaying());

		if (ImGui::TreeNode("Box")) {
			box_.Debug();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere")) {
			sphere_.Debug();
			ImGui::TreePop();
		}

		ImGui::End();

#endif // _DEBUG

		if (input_->GetKeyboard()->Trigger(DIK_Q)) {
			//SE再生
			se1_.Play(0.5f, false);
		}

		if (input_->GetKeyboard()->Trigger(DIK_W)) {
			//一時停止
			se1_.Pause();
		}

		if (input_->GetKeyboard()->Trigger(DIK_E)) {
			//再開
			se1_.ReStart();
		}

		if (input_->GetKeyboard()->Trigger(DIK_R)) {
			//停止
			se1_.Stop();
		}

		if (input_->GetKeyboard()->Trigger(DIK_1)) {
			//ボリューム調整
			se1_.SetVolume(0.1f);
		}

	}

	//Particle3D
	{

		for (int32_t i = 0; i < 32; i++) {
			//ビルボードフラグ
			particle_->isBillboard_ = true;
			//モデル一つ一つのアクティブフラグ
			particle_->isActive_[i] = true;
			//トランスフォーム
			particle_->transforms_[i].translate_ = { i * 0.1f, 0.0f,0.0f };
			particle_->transforms_[i].scale_ = { 1.0f,1.0f,1.0f };
			particle_->transforms_[i].rotateQuaternion_ = MLEngine::Math::IdentityQuaternion();
			//色
			particle_->colors_[i] = { 1.0f, i / 32.0f, 1.0f, 1.0f };
		}

	}

	camera_.Update();

	lineBox_.Update();
	lineSphere_.Update();

}

void DebugScene::Draw()
{

	model_.Draw(&camera_);

	particle_->Draw(&camera_);

	lineSphere_.Draw(&camera_);

	lineBox_.Draw(&camera_);

}

void DebugScene::DrawImgui(){
	ImGui::Begin("お試し");

	ImGui::Text("テスト");

	ImGui::End();

}
