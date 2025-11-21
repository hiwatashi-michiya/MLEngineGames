#include "DebugScene.h"
#include "ImguiManager.h"
#include"Externals/imgui/imgui.h"
#include"SceneManager.h"
#include"PlayScene.h"

using namespace MLEngine::Math;
using namespace MLEngine::Resource;
using namespace MLEngine::Object::Collision;

DebugScene::DebugScene() {
	input_ = MLEngine::Input::Manager::GetInstance();
}

DebugScene::~DebugScene()
{
}

inline void DebugScene::Initialize()
{
	//お試しプッシュ

#pragma region
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
#pragma endregion

#pragma region
	joyconInput = std::make_unique<Joycon>();
	joyconInput->Init();
#pragma endregion ジョイコン

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{
	joyconInput->Update();
	//トランスフォーム
	Matrix4x4 result;
	result = MakeAffineMatrix(Vector3(1.0f,1.0f,1.0f), ConvertFromEuler(Vector3(0.0f, 0.0f, 0.0f)),Vector3(0.0f,0.0f,0.0f));

	if (input_->GetKeyboard()->Trigger(DIK_SPACE)) {
		sceneManager_->ChangeScene(new PlayScene());
	}

	camera_.Update();

	lineBox_.Update();
	lineSphere_.Update();
}

void DebugScene::Draw()
{

	model_.Draw(&camera_);

	//particle_->Draw(&camera_);

	//lineSphere_.Draw(&camera_);

	//lineBox_.Draw(&camera_);

}


void DebugScene::DrawImgui() {
#ifdef _DEBUG


	ImGui::Begin("お試し");

	ImGui::Text("テスト");

	ImGui::End();
#endif // _DEBUG

}
